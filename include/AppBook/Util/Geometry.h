/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code IsIn this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once 


#include "AppBook/Util/StrAcc.h"





struct APPBOOK_EXPORTS Point
{

    int X = -1;
    int Y = -1;

    using Array = std::vector<Point>;
    using Iterator = Point::Array::iterator;
    using CIterator = Point::Array::const_iterator;

    Point()= default;
    Point(Point&&) noexcept = default;
    ~Point() = default;

    Point(int x_, int y_) : X(x_), Y(y_) {}
    Point(const Point& p) = default;


    Point& operator = (const Point& p) = default;
//    {
//        X = p.X;
//        Y = p.Y;
//        return *this;
//    }

    Point& operator-= (const Point& dxy)
    {
        X -= dxy.X;
        Y -= dxy.Y;
        return *this;
    }


    Point& operator+= (const Point& dxy)
    {
        X += dxy.X;
        Y += dxy.Y;
        return *this;
    }
    bool operator == (Point Rhs) const{ return (X == Rhs.X)&& (Y == Rhs.Y);}

    Point operator+ (const Point& dxy) const
    {
        return {dxy.X + X, dxy.Y + Y};
    }

    [[nodiscard]] Point Max (const Point& b) const;
    [[nodiscard]] Point Min (const Point& b) const;
    Point operator - (const Point& Rhs)     const{return {X-Rhs.X,Y-Rhs.Y};}
    [[nodiscard]] Point Lesser(Point Rhs)   const{return {X<Rhs.X?X:Rhs.X,Y<Rhs.Y?Y:Rhs.Y};}
    [[nodiscard]] Point Greater(Point Rhs)  const{return {X>Rhs.X?X:Rhs.X,Y>Rhs.Y?Y:Rhs.Y};}
    Point operator << (Point Rhs) const{ return Lesser(Rhs); }
    Point operator >> (Point Rhs) const{ return Greater(Rhs); }

    Point& operator()(int x_, int y_)
    {
        X = x_;
        Y = y_;
        return *this;
    }
    operator std::string() const
    {
        StrAcc str = "{%d,%d}";
        str << X << Y;
        return str();
    }
    };


struct APPBOOK_EXPORTS Dim
{
    int W = 0;
    int H = 0;

    Point Min   {};
    Point Max   {};

    operator bool() const { return ((W > 0) && (H > 0)); }
    operator std::string() const;

    bool operator< (const Dim& dwh) const;
    bool operator== (const Dim& dwh) const;
    bool operator> (const Dim& dwh) const;
    bool operator!= (const Dim& dwh) const;

    [[nodiscard]] bool SetPolicy() const { return !((Min.X <= 0) && (Min.Y <= 0) && (Max.X <= 0) && (Max.Y <= 0)); }

    [[nodiscard]] auto Area() const { return W * H; }
    };


// Assign : {A.X(X), A.Y(Y), B.X(X + W - 1), B.Y(Y + H - 1)}
// Width{ return  B.X - A.X + 1; }

struct APPBOOK_EXPORTS Rect
{

    Point A{0,0};
    Point B{0,0};
    Point Cursor{0,0}; ///< Inner Cursor Coordinates used by the location operations

    Dim   Dwh;
    bool NoWrap = false;

    using Array = std::vector<Rect>;
    using Iterator = Rect::Array::iterator;
    using CIterator = Rect::Array::const_iterator;

    Rect() = default;
    ~Rect() = default;
    Rect(Rect&& r) noexcept = default;
    Rect(const Rect& r) = default;


    Rect& operator=(Rect&& r) noexcept = default;
    Rect& operator=(const Rect& r) = default;

    Rect(const Point& a_, const Point& b_)
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
    explicit Rect(const Dim& dxy)
    {
        A = {};
        Dwh = dxy;
        B = {A.X + Dwh.W - 1, A.Y + Dwh.H - 1};
    }

    /*!
     * \brief Construct A Rect from the Dim values, at Point a_.
     * \param a_
     * \param d
     */
    Rect(const Point& a_, const Dim& d)
    {
        A = a_;
        Dwh = d;
        B = {A.X + d.W - 1, A.Y + d.H - 1};
    }

    void Assign(int x, int y, int w, int h)
    {
        A = {x, y};
        B = {x + w - 1, y + h - 1};
        Dwh = {w, h};
    }

    void SetTopLeft(Point pt);

    void Assign(const Point& a_, const Point& b_)
    {
        A = a_;
        B = b_;
        Dwh.W = B.X - A.X + 1;
        Dwh.H = B.Y - A.Y + 1;
    }

    void Assign(const Point& a_, const Dim& dxy)
    {
        A = a_;
        Dwh = dxy;
        B = {A.X + dxy.W - 1, A.Y + dxy.H - 1};
    }


    Rect& operator+= (const Point& dx)
    {
        A += dx;
        B += dx;
        return *this;
    }
    Rect& operator-= (const Point& dx)
    {
        A -= dx;
        B -= dx;
        return *this;
    }
    void Resize(const Dim& new_sz)
    {
        Assign({A.X, A.Y}, new_sz);
    }

    void MoveAt(const Point& p)
    {
        A.X = p.X;
        A.Y = p.Y;
        B.X = A.X + Dwh.W - 1;
        B.Y = A.Y + Dwh.H - 1;
    }


    bool IsIn(const Point& Pt) const
    {
        return ((Pt.X >= A.X) && (Pt.X <= B.X) && (Pt.Y >= A.Y) && (Pt.Y <= B.Y));
    }


    void Move(const Point& DeltaPt)
    {
        A += DeltaPt;
        B += DeltaPt;
    }

    [[nodiscard]] int Width()  const { return Dwh.W; }
    [[nodiscard]] int Height() const  { return Dwh.H; }

    bool operator[](const Point& Pt) const
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
    [[nodiscard]] Rect operator & (const Rect& R) const
    {
        auto TopL = R.A - A;
        auto BotR = R.B - B;

        Rect Tmp;
        Tmp.A = {std::max(Tmp.A.X, R.A.X), std::max(Tmp.A.Y, R.A.Y)};
        Tmp.B = {std::min(B.X, R.B.X), std::min(B.Y, R.B.Y)};

        auto c = IsIn(Tmp.A) || IsIn(Tmp.B);
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

    /*!
     * \brief operator / Get the intersection between this and rhs then get the origin of this from rhs
     * \param rhs
     * \return result Rect
     */
    Rect operator/ (const Rect& rhs) const
    {
        Rect Tmp = *this & rhs;
        Tmp -= A;
        return Tmp;
    }
    /*!
        @brief merges this and R
    */
    Rect operator| (const Rect& R) const
    {
        Rect Tmp;
        Point a_ = {R.A.X <= A.X ? R.A.X : A.X, R.A.Y <= A.Y ? R.A.Y : A.Y};
        Point b_ = {R.B.X <= B.X ? R.B.X : B.X, R.B.Y <= B.Y ? R.B.Y : B.Y};
        Tmp.Assign(a_, b_);
        return Tmp;
    }

    Rect operator+ (const Point& Pt)const {
        Rect Tmp = *this;
        Tmp.A.X += Pt.X;
        Tmp.A.Y += Pt.Y;
        Tmp.B.X += Pt.X;
        Tmp.B.Y += Pt.Y;
        Tmp.Dwh = Dwh;
        return Tmp;
    }

    [[nodiscard]] std::string to_string() const;
    operator std::string() const;
    operator bool() const
    {

        return Dwh.operator bool();
    }

    //------------- location operations ---------------------------------
    void Home();
    bool operator ++();
    Point operator ++(int);
    bool operator --();
    Point operator --(int);
    bool GotoXY(Point P);
    Point operator *() const { return Cursor; }

};


struct APPBOOK_EXPORTS WinBuffer
{
    StrAcc * Win = nullptr;
    Point Cxy;
    Rect R;

    WinBuffer& GotoXY(int x, int y);
    WinBuffer& operator<< (Point xy);
    void SetGeometry(int w, int h);

    // internal cursor movements:
    WinBuffer& operator++();
    void operator++(int);
    WinBuffer& operator--();
    void operator--(int);
    // --------------------------
    template<typename T> WinBuffer& operator<< (T v)
    {
        StrAcc str;
        str << v;
        return Put(str());
    }




    WinBuffer& Put(const std::string& txt);

    void Clear();
    void Release();
    std::string Setails();
    operator std::string();// {return Win ? Win->str() : "";}

    };


