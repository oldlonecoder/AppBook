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





struct APPBOOK_API Point
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
    Point& operator-= (const Point& dxy);
    Point& operator+= (const Point& dxy);
    bool operator == (Point Rhs) const;
    Point operator+ (const Point& dxy) const;
    [[nodiscard]] Point Max (const Point& b) const;
    [[nodiscard]] Point Min (const Point& b) const;
    Point operator - (const Point& Rhs)     const;
    [[nodiscard]] Point Lesser(Point Rhs)   const;
    [[nodiscard]] Point Greater(Point Rhs)  const;
    Point operator << (Point Rhs) const;
    Point operator >> (Point Rhs) const;

    Point& operator()(int x_, int y_);
    operator std::string() const;
    };


struct APPBOOK_API Dim
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

    [[nodiscard]] bool SetPolicy() const;

    [[nodiscard]] int32_t Area() const;
};




/*!
 * @brief Rectangular Geometry object (inspired by the Qt project, many, many years ago).
 *
 * It has evolved with my own point of view and limitations.
 * @li Provides internal Cursor relative coordinates
 * @li Still needs more limit implementations (min-max restrictions helpers)
 *
 * @author oldlonecoder (lussier.serge@gmail.com)
 *
 */
struct APPBOOK_API Rect
{

    Point A{0,0};
    Point B{0,0};
    Point Cursor{0,0}; ///< Inner Cursor Coordinates used by the location operations, relative to the Rect::A (TopLeft) coord.

    Dim   Dwh;
    bool NoWrap = false;

    using Array = std::vector<Rect>;
    using Iterator = Rect::Array::iterator;
    using CIterator = Rect::Array::const_iterator;

    Rect() = default;
    ~Rect() = default;
    Rect(Rect&& r) noexcept = default;
    Rect(const Rect& r) = default;
    Rect(const Point& a_, const Point& b_);
    Rect(const Point& a_, const Dim& d);
    explicit Rect(const Dim& dxy);

    Rect& operator=(Rect&& r) noexcept = default;
    Rect& operator=(const Rect& r) = default;

    void Assign(int x, int y, int w, int h);
    void Assign(const Point& a_, const Point& b_);
    void Assign(const Point& a_, const Dim& dxy);

    void SetTopLeft(Point pt);

    Rect  Grow(Point DXY);
    Rect& operator+= (const Point& dx);
    Rect& operator-= (const Point& dx);
    void Resize(const Dim& new_sz);
    void MoveAt(const Point& p);
    bool In(const Point& Pt) const;
    void Move(const Point& DeltaPt);

    [[nodiscard]] int Width()  const { return Dwh.W; }
    [[nodiscard]] int Height() const  { return Dwh.H; }
    bool operator[](const Point& Pt) const;


    [[nodiscard]] Rect operator & (const Rect& R) const;

    /*!
     * \brief operator / Get the intersection between this and rhs then get the origin of this from rhs
     * \param rhs
     * \return result Rect
     */
    Rect operator/ (const Rect& rhs) const;
    /*!
        @brief merges this and R
    */
    Rect operator| (const Rect& R) const;
    Rect operator+ (const Point& Pt)const;
    [[nodiscard]] std::string to_string() const;
    operator std::string() const;
    operator bool() const{return (Dwh);}//.operator bool();

    //------------- location operations ---------------------------------
    void Home();
    bool operator ++();
    bool operator ++(int);
    bool operator --();
    bool operator --(int);
    bool GotoXY(Point XY);
    Point operator *() const { return Cursor; }

};


struct APPBOOK_API WinBuffer
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


