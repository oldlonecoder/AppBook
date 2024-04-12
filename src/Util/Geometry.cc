//
// Created by oldlonecoder on 10/29/23.
//

#include "AppBook/Util/Geometry.h"
#include <AppBook/Book/AppBook.h>







bool Dim::operator < (const Dim& dwh) const
{
    return dwh.Area() < Area();
}

bool Dim::operator == (const Dim& dwh) const
{
    return Area() == dwh.Area();
}

bool Dim::operator > (const Dim& dwh) const
{
    return Area() > dwh.Area();
}

bool Dim::operator != (const Dim& dwh) const
{
    return Area() != dwh.Area();
}



//Rect::Rect(Rect&& R) noexcept:
//    A(std::move(R.A)),
//    B(std::move(R.B)),
//    Dwh(std::move(R.Dwh))
//{

//}


//Rect::Rect(const Rect& R) :
//    A(R.A),
//    B(R.B),
//    Dwh(R.Dwh)
//{
//
//}


//Rect& Rect::operator=(Rect&& R) noexcept
//{
//    A = std::move(R.A);
//    B = std::move(R.B);
//    Dwh = std::move(R.Dwh);
//    return *this;
//}

void Rect::SetTopLeft(Point pt)
{
    A = pt;
    B = {A.X + Dwh.H - 1, A.Y + Dwh.H - 1 };
}


//Rect& Rect::operator=(const Rect& R)
//{
//    A = R.A;
//    B = R.B;
//    Dwh = R.Dwh;
//    return *this;
//
//}


Dim::operator std::string() const
{
    //StrAcc str = "{{%d,%d}, {%d,%d}, %d %d}: %d";
    //str << min.X << min.Y << max.X << max.Y << W << H << W*H;
    StrAcc str;
    if(SetPolicy())
    {
        str = "(%d/(%d,%d),%d/(%d,%d)):%d";
        str << W << Min.X << Max.X << H << Min.Y << Max.Y << W * H;
    }
    else
    {
        str = "(%d,%d):%d";
        str << W << H << W * H;
    }
    return  str();
}

bool Dim::SetPolicy() const
{
    return !((Min.X <= 0) && (Min.Y <= 0) && (Max.X <= 0) && (Max.Y <= 0));
}

int32_t Dim::Area() const
{
    return W * H;
}


//- -------------------------------- WinBuffer ------------------------------------------------------------


WinBuffer &WinBuffer::GotoXY(int x, int y)
{
    Cxy = {x, y};
    return *this;
}

void WinBuffer::SetGeometry(int w, int h)
{
    R = Rect(Point(0, 0), Dim{w, h});

    Clear();
}


WinBuffer &WinBuffer::operator++()
{
    if(Cxy.X >= R.Dwh.W)
    {
        if(Cxy.Y <= R.Dwh.H)
        {
            Cxy.Y++;
            Cxy.X = 0;
        }
    }
    else
        Cxy.X++;

    return *this;
}

void WinBuffer::operator++(int)
{
    if(Cxy.X >= R.Dwh.W)
    {
        if(Cxy.Y <= R.Dwh.H)
        {
            Cxy.Y++;
            Cxy.X = 0;
        }
    }
    else
        Cxy.X++;
}

WinBuffer &WinBuffer::operator--()
{

    return *this;
}

void WinBuffer::operator--(int)
{
    //return *this;
}

/// <summary>
/// Serge Lussier
/// p = 3
/// 10-13 + 3;
///        *   Serge L*
/// </summary>
/// <param name="txt"></param>
/// <returns></returns>
WinBuffer &WinBuffer::Put(const std::string &txt)
{
    auto line_width = R.Width() - Cxy.X;
    auto ln = txt.length();

    auto dx = line_width <= ln ? line_width : ln;

    std::string::iterator crs = Win->StdStr().begin() + Cxy.Y * R.Width() + Cxy.X;
    auto p   = txt.begin();
    for(size_t x = 0; x < dx; x++)
        *crs++ = *p++;

    Cxy.X += line_width;

    return *this;
}

void WinBuffer::Clear()
{
    if(!Win)
        Win = new StrAcc;
    else
        Win->Clear();

    *Win = std::string(R.Width() * R.Height(), ' ');
    //std::cerr << __PRETTY_FUNCTION__ << "contents: '" << Win->_InputText << "'\n";
}

void WinBuffer::Release()
{
    delete Win;
}

std::string WinBuffer::Setails()
{
    StrAcc str = "WinBuffer Setails:\n";
    str << R.to_string() << " cursor: " << std::string(Cxy);
    return str();
}

WinBuffer::operator std::string()
{
    std::string str;
    if(!Win)
        return "oops! there is no such defined and initialised Area!";
    str += '\n';
    for(int l = 0; l < R.Height(); l++)
    {
        for(int c = 0; c < R.Width(); c++)
            str += *(Win->StdStr().begin() + (l * R.Width() + c));
        str += '\n';
    }
    return str;
}


Rect::operator std::string() const
{
    StrAcc str = "(%d,%d)[{%d,%d} {%d,%d}]:{%d*%d}[%d]";
    str << Cursor.X << Cursor.Y << A.X << A.Y << B.X << B.Y << Dwh.W << Dwh.H << Dwh.W * Dwh.H;
    return str();
    // return std::format("[{{{},{}}} {{{},{}}}]:{{{}*{}}}[{}]", A.X, A.Y, B.X, B.Y, sz.W, sz.H, sz.W * sz.H);
}

std::string Rect::to_string() const
{
    return this->operator std::string();
}

void Rect::Home()
{
    Cursor = A;
}

bool Rect::operator++()
{
    ++Cursor.X;
    if(Cursor.X >= Dwh.W)
    {
        if(NoWrap)
        {
            --Cursor.X;
            return false;
        }
        ++Cursor.Y;
        if(Cursor.Y > Dwh.H)
        {
            --Cursor.Y;
            Cursor.X = Dwh.W-1;
            Book::Status() << " Cursor wraps to Home.";
            Cursor = A;
            return false;
        }
        Cursor.X = A.X;
    }
    return true;
}


bool Rect::operator++(int){ return ++(*this); }

bool Rect::operator--()
{
    --Cursor.X;
    if(Cursor.X < 0)
    {
        if(NoWrap)
        {
            ++Cursor.X;
            return false;
        }
        --Cursor.Y;
        if(Cursor.Y < 0)
        {
            Cursor.Y = 0;
            return false;
        }
        Cursor.X = 0;
    }
    return true;
}


bool Rect::operator--(int)
{
    return --(*this);
}


/*!
 * @brief
 * @param P
 * @return
 * @note If Cursor < R.A then P will not be assigned.
 */
bool Rect::GotoXY(Point XY)
{
    if(!In(XY)) return false;
    Cursor = XY;
    return true;
}

Rect::Rect(const Point &a_, const Point &b_)
{
    A = a_;
    B = b_;
    Dwh.W = B.X - A.X + 1;
    Dwh.H = B.Y - A.Y + 1;
}



/*!
 * \brief Construct A Rect from the Dim values.
 * \param wh
 */
Rect::Rect(const Dim &dxy)
{
    A = {0,0};
    Dwh = dxy;
    B = {A.X + Dwh.W - 1, A.Y + Dwh.H - 1};
}


/*!
 * \brief Construct A Rect from the Dim values, at Point a_.
 * \param a_
 * \param d
 */
Rect::Rect(const Point &a_, const Dim &d)
{
    A = a_;
    Dwh = d;
    B = {A.X + d.W - 1, A.Y + d.H - 1};
}

void Rect::Assign(int x, int y, int w, int h)
{
    A = {x, y};
    B = {x + w - 1, y + h - 1};
    Dwh = {w, h};
}

void Rect::Assign(const Point &a_, const Point &b_)
{
    A = a_;
    B = b_;
    Dwh.W = B.X - A.X + 1;
    Dwh.H = B.Y - A.Y + 1;
}

void Rect::Assign(const Point &a_, const Dim &dxy)
{
    A = a_;
    Dwh = dxy;
    B = {A.X + dxy.W - 1, A.Y + dxy.H - 1};
}

Rect &Rect::operator+=(const Point &dx)
{
    A += dx;
    B += dx;
    return *this;
}

Rect &Rect::operator-=(const Point &dx)
{
    A -= dx;
    B -= dx;
    return *this;
}

void Rect::Resize(const Dim &new_sz)
{
    Assign({A.X, A.Y}, new_sz);
}

void Rect::MoveAt(const Point &p)
{
    A.X = p.X;
    A.Y = p.Y;
    B.X = A.X + Dwh.W - 1;
    B.Y = A.Y + Dwh.H - 1;
}

bool Rect::In(const Point &Pt) const
{
    return ((Pt.X >= A.X) && (Pt.X <= B.X) && (Pt.Y >= A.Y) && (Pt.Y <= B.Y));
}

void Rect::Move(const Point &DeltaPt)
{
    A += DeltaPt;
    B += DeltaPt;
}

bool Rect::operator[](const Point &Pt) const
{
    return ((Pt.X >= A.X) && (Pt.X <= B.X) && (Pt.Y >= A.Y) && (Pt.Y <= B.Y));
}



/*!
    @brief intersection between this (A) and R (B).

    @note A & B must be on the same referential offset. Undefined behaviour otherwise.
    @author &copy; 1996, 2023, Serge Lussier, (oldlonecoder'@'gmail.com)
    @code
   A+==============================+
    |                              |
    |  B+==========================|===========+
    |   |                          |           |
    |   |                          |           |
    +==============================+           |
        |                                      |
        |                                      |
        +======================================+
    @endcode
*/
Rect Rect::operator&(const Rect &R) const
{
    auto TopL = R.A - A;
    auto BotR = R.B - B;

    Rect Tmp;
    Tmp.A = {std::max(Tmp.A.X, R.A.X), std::max(Tmp.A.Y, R.A.Y)};
    Tmp.B = {std::min(B.X, R.B.X), std::min(B.Y, R.B.Y)};

    auto c = In(Tmp.A) || In(Tmp.B);
    //Tmp += B.Tmp;
    if (!c)
        Tmp.Dwh = {};
    else
    {
        Tmp.Dwh.W = Tmp.B.X - Tmp.A.X + 1;
        Tmp.Dwh.H = Tmp.B.Y - Tmp.A.Y + 1;
    }
    // Need To check again...
    if ((Tmp.Dwh.H <= 0) || (Tmp.Dwh.W <= 0))
        Tmp.Dwh = {};

    return Tmp;
}

Rect Rect::operator/(const Rect &rhs) const
{
    Rect Tmp = *this & rhs;
    Tmp -= A;
    return Tmp;
}

Rect Rect::operator|(const Rect &R) const
{
    Rect Tmp;
    Point a_ = {R.A.X <= A.X ? R.A.X : A.X, R.A.Y <= A.Y ? R.A.Y : A.Y};
    Point b_ = {R.B.X <= B.X ? R.B.X : B.X, R.B.Y <= B.Y ? R.B.Y : B.Y};
    Tmp.Assign(a_, b_);
    return Tmp;
}

Rect Rect::operator+(const Point &Pt) const
{
    Rect Tmp = *this;
    Tmp.A.X += Pt.X;
    Tmp.A.Y += Pt.Y;
    Tmp.B.X += Pt.X;
    Tmp.B.Y += Pt.Y;
    Tmp.Dwh = Dwh;
    return Tmp;
}

Rect Rect::Grow(Point DXY)
{
    Rect R = *this;

    R.A.X -= DXY.X; R.B.X += DXY.X;
    R.A.Y -= DXY.Y; R.B.Y += DXY.Y;
    R.Dwh.W = B.X - A.X + 1;
    R.Dwh.H = B.Y - A.Y + 1;
    return R;
}


WinBuffer & WinBuffer::operator<<(Point xy)
{
    GotoXY(xy.X, xy.Y);
    return *this;
}

Point Point::Max(const Point &b) const
{
    Point c;
    if(X <= b.X) c.X = b.X; else c.X = X;
    if(Y <= b.Y) c.Y = b.Y; else c.Y = Y;
    return c;
}

Point Point::Min(const Point &b) const
{
    Point c;
    if(X >= b.X) c.X = b.X; else c.X = X;
    if(Y >= b.Y) c.Y = b.Y; else c.Y = Y;
    return c;
}

Point &Point::operator-=(const Point &dxy)
{
    X -= dxy.X;
    Y -= dxy.Y;
    return *this;
}

Point &Point::operator+=(const Point &dxy)
{
    X += dxy.X;
    Y += dxy.Y;
    return *this;
}

bool Point::operator==(Point Rhs) const
{
    return (X == Rhs.X)&& (Y == Rhs.Y);
}

Point Point::operator+(const Point &dxy) const
{
    return {dxy.X + X, dxy.Y + Y};
}

Point Point::operator-(const Point &Rhs) const
{
    return {X-Rhs.X,Y-Rhs.Y};
}

Point Point::Lesser(Point Rhs) const
{
    return {X<Rhs.X?X:Rhs.X,Y<Rhs.Y?Y:Rhs.Y};
}

Point Point::Greater(Point Rhs) const
{
    return {X>Rhs.X?X:Rhs.X,Y>Rhs.Y?Y:Rhs.Y};
}

Point Point::operator<<(Point Rhs) const
{
    return Lesser(Rhs);
}

Point Point::operator>>(Point Rhs) const
{
    return Greater(Rhs);
}

Point &Point::operator()(int x_, int y_)
{
    X = x_;
    Y = y_;
    return *this;
}

Point::operator std::string() const
{
    StrAcc str = "{%d,%d}";
    str << X << Y;
    return str();
}




