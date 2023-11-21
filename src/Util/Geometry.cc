//
// Created by oldlonecoder on 10/29/23.
//

#include "AppBook/Util/Geometry.h"







bool Dim::operator < (const Dim& dwh) const
{
    return dwh.Area() > Area();
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
    StrAcc str = "[{%d,%d} {%d,%d}]:{%d*%d}[%d]";
    str << A.X << A.Y << B.X << B.Y << Dwh.W << Dwh.H << Dwh.W * Dwh.H;
    return str();
    // return std::format("[{{{},{}}} {{{},{}}}]:{{{}*{}}}[{}]", A.X, A.Y, B.X, B.Y, sz.W, sz.H, sz.W * sz.H);
}

std::string Rect::to_string() const
{
    return this->operator std::string();
}

void Rect::Home()
{
    Cursor = {0,0};
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
            return false;
        }
        Cursor.X = 0;
    }
    return true;
}


Point Rect::operator++(int)
{
    Point P = Cursor;
    if(!++(*this)) P={-1,-1};
    return P;
}

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

Point Rect::operator--(int)
{
    Point P = Cursor;
    if(!--(*this)) P={-1,-1};
    return P;
}

bool Rect::GotoXY(Point P)
{
    if(IsIn(P)) return false;
    Cursor = P;
    return true;
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




