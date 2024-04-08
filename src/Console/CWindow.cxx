//
// Created by oldlonecoder on 24-04-08.
//

#include "AppBook/Console/CWindow.h"
#include <AppBook/Book/AppBook.h>

/******************************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                                        *
 *   serge.lussier@oldlonecoder.club                                                      *
 *                                                                                        *
 *                                                                                        *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge Lussier)                                                        *
 *   and no one else then not even {copilot, chatgpt, or any other AI calamities}         *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply To the Code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/

//#pragma once



// Boook !@! lol


namespace Book::ConIO
{

#pragma region CWindow_Char


CWindow::Char::Char(CWindow::Char::Type _Ch): Mem(_Ch){}
CWindow::Char::Char(CWindow::Char* _C): Mem(_C->Mem){}

CWindow::Char &CWindow::Char::SetFg(Color::Code fg_)
{
    Mem = (Mem & ~FGMask) | (static_cast<Char::Type>(fg_) << FGShift);
    return *this;
}

CWindow::Char &CWindow::Char::SetBg(Color::Code bg_)
{
    Mem = (Mem & ~BGMask) | (static_cast<Char::Type>(bg_) << BGShift);
    return *this;
}

[[maybe_unused]] CWindow::Char &CWindow::Char::SetAttributes(CWindow::Char::Type d_)
{
    Mem = (Mem & ~AttrMask) | d_;
    return *this;
}


[[maybe_unused]] CWindow::Char &CWindow::Char::SetColors(Color::Pair &&c_)
{
    return SetFg(c_.Fg).SetBg(c_.Bg);
}

[[maybe_unused]] CWindow::Char &CWindow::Char::SetColors(Color::Pair &c_)
{
    return SetFg(c_.Fg).SetBg(c_.Bg);
}

CWindow::Char &CWindow::Char::operator=(CWindow::Char::Type d_)
{
    Mem = d_;
    return *this;
}

CWindow::Char &CWindow::Char::operator=(CWindow::Type d_)
{
    Mem = *d_;
    return *this;
}

CWindow::Char &CWindow::Char::operator=(char d_)
{
    Mem = (Mem & ~CharMask) | d_;
    return *this;
}

Color::Code CWindow::Char::Fg() const
{
    return static_cast<Color::Code>((Mem & FGMask) >> FGShift);
}

Color::Code CWindow::Char::Bg() const
{
    return static_cast<Color::Code>((Mem & BGMask) >> BGShift);
}

[[maybe_unused]] Color::Pair CWindow::Char::Colors() const
{
    return {Fg(),Bg()};
}

[[maybe_unused]] Utf::Glyph::Type CWindow::Char::IconID() const
{
    if (!(Mem & Char::UGlyph)) return Utf::Glyph::Poop;
    auto Ic = Mem & Char::CharMask;
    if (Ic > 103) Ic = Utf::Glyph::Bug;
    return  Ic;
}

[[maybe_unused]] Utf::AccentFR::Type CWindow::Char::AccentID() const
{
    auto AID = Mem & Char::CharMask;
    if (AID > Utf::AccentFR::Ucirc) return Utf::AccentFR::Agrave;
    return static_cast<Utf::AccentFR::Type>(AID);
}

uint8_t CWindow::Char::Ascii() const
{
    return Mem & CharMask;
}

[[maybe_unused]] uint16_t CWindow::Char::Attributes() const
{
    return (Mem & AttrMask) >> ATShift;
}

CWindow::Char &CWindow::Char::operator<<(Utf::Glyph::Type i_)
{
    Mem = (Mem & ~CharMask) | i_ | UGlyph;
    return *this;
}

CWindow::Char &CWindow::Char::operator<<(Utf::AccentFR::Type a_)
{
    Mem = (Mem & ~CharMask) | a_ | Accent;
    return *this;
}

[[maybe_unused]] std::string CWindow::Char::RenderColors() const
{
    std::string str;
    str += Color::Ansi({Fg(),Bg()});
    return str;
}

[[maybe_unused]] CWindow::Char &CWindow::Char::ResetAttributes(CWindow::Char::Type bits_)
{
    Book::Status() << " This feature is not determined in this context, then not implemented yet";
    return *this;
}


CWindow::Char::operator std::string() const
{
    return Details();
}

std::string CWindow::Char::Details() const
{
    StrAcc Str = "%s[Char:'%s%d%s'/Fg:'%s%s%s'/Bg:'%s%s%s']";
    Str << Color::White <<Color::Yellow << (int)Ascii() << Color::White
        << Color::Yellow << Color::Name(Fg()) << Color::White
        << Color::Yellow << Color::Name(Bg()) << Color::White;

    return Str();
}


#pragma endregion Char


//

//
//CWindow::CWindow(Object *ParentObject): OwnerParent(ParentObject){}
//
//CWindow::~CWindow()
//{
//    delete [] MemoryBloc;
//}
//
//Dim CWindow::Dimensions()
//{
//
//    return BlocGeometry.Dwh;
//}
//
//CWindow::Char* CWindow::Alloc(Dim dim_)
//{
//    if(dim_)
//        BlocGeometry.Assign({}, dim_);
//
//    delete [] MemoryBloc;
//    MemoryBloc = new Char [dim_.W * dim_.H + 1];
//    return MemoryBloc;
//}
//
//
//CWindow::Char* CWindow::Realloc(Dim dim_)
//{
//    if(!dim_)
//        throw AppBook::Exception()[ AppBook::Except() << Book::Result::Unexpected << " unset Dimensions" ];
//
//    BlocGeometry.Assign({},dim_);
//
//    delete [] MemoryBloc;
//    MemoryBloc = new Char[dim_.W * dim_.H + 1];
//    return MemoryBloc;
//}
//
//
//
//void CWindow::Dealloc()
//{
//    delete [] MemoryBloc;
//    MemoryBloc = nullptr;
//    BlocGeometry = {};
//}
//
//
//CWindow::Char* CWindow::Peek(const Point &pt_)
//{
//    auto* p = MemoryBloc + (pt_.X + pt_.Y * BlocGeometry.Dwh.W);
//    if (!BlocGeometry[pt_])
//        return nullptr;
//
//    return p;
//}
//
//Rect CWindow::Geometry()
//{
//    return BlocGeometry;
//}
//
//int CWindow::Width() const
//{
//    return BlocGeometry.Dwh.W;
//}
//
//int CWindow::Height() const
//{
//    return BlocGeometry.Dwh.H;
//}
//
//
//
//Book::Result CWindow::Clear()
//{
//    if (!MemoryBloc)
//        throw AppBook::Exception()[ AppBook::Fatal() << " Attempt to clear a null VDC"];
//
//    auto* C = MemoryBloc;
//    auto blk = BlocGeometry.Dwh.W * BlocGeometry.Dwh.H;
//    for (int x = 0; x < blk; x++) *C++ = D;
//    return Book::Result::Accepted;
//}
//
//Book::Result CWindow::Clear(const Rect &r_)
//{
//    Rect R;
//    if(r_)
//        R = Rect({}, BlocGeometry.Dwh) & r_;
//    else
//        return Clear();
//    if(!MemoryBloc)
//        throw AppBook::Exception()[AppBook::Fatal() << "Attempt to clear a null CWindow"];
//
//    if(!R)
//        return Book::Result::Rejected;
//
//    for(int y = R.A.Y; y <= R.B.Y; y++)
//    {
//        Char* C = Peek({R.A.X, y});
//        for(int x = 0; x < R.Width(); x++) *C++ = D;
//    }
//
//    return Book::Result::Accepted;
//}
//
//
//
//Book::Result CWindow::Clear(CWindow::Char::Type a_, const Rect &r_)
//{
//    auto A = D;
//    D = a_;
//    auto Ok = Clear(r_);
//    D = A;
//    return Ok;
//}
//
//
//CWindow::Painter::Painter(CWindow *DC, Rect R, CWindow::Char* Attr):Dc(DC), Wh(R), DefAttr(Attr)
//{
//
//}
//
//CWindow::Painter &CWindow::Painter::operator<<(Color::Code aColorID)
//{
//    Char.SetFg(aColorID);
//    return *this;
//}
//
//
//
///**
// * @brief Assigns the IconAttr to the cell's attr bit field and the Icon Code ID to the ascci bit field
// *
// * @param aIconID
// * @return painter& ( ref to self )
// * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
// */
//CWindow::Painter &CWindow::Painter::operator<<(Utf::Glyph::Type aIconID)
//{
//    CWindow::Char Cell = Char;
//
//    *Cursor = (Cell << aIconID).Mem;
//    Cell = *Cursor;
//    ++Cursor;
//    Char = Cell;
//    *Cursor = (Char << ' ').Mem;
//
//    return *this;
//}
//
///**
// * @brief Writes string \a aStr into the painter's backbuffer vdc
// *
// * @param aStr
// * @return painter& (ref to self )
// *
// * @note This painter version does not handle justify flags, nor checks the logical line and columns boundaries - Only the linear-bloc limit.
// * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
// */
//CWindow::Painter &CWindow::Painter::operator<<(const std::string &aStr)
//{
//    Point Pt = Position();
//    auto C = aStr.begin();
//    //diagnostic::debug() << "painter <<  \"" << aStr << "\" @" << (std::string)pt << " -> in " << _r.to_string();
//    while ((C != aStr.end()) && Wh[Pt])
//    {
//        *Cursor = (Char << *C++).Mem; // Ouais... Pas mal pété; A bit risky, doing three things in the very same statement...
//        ++Cursor;
//        Pt.X++;
//    }
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::operator<<(const StrAcc &aStr)
//{
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::operator<<(const Point &XY)
//{
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::operator<<(const char *aStr)
//{
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::operator<<(Utf::AccentFR::Type aAcc)
//{
//    return *this;
//}
//
//
///**
// * @brief operator << overload acting as Painter::GotoXY
// *
// * @param XY coordinate point into the painter's subregion.
// * @return CWindow::Painter& ( ref to self )
// * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
// */
//CWindow::Painter &CWindow::Painter::GotoXY(Point XY)
//{
//    Point Pt = XY + Wh.A; // + {0,0} by default;
//    if (!Wh[Pt])
//        throw AppBook::Exception()[AppBook::Fatal() << Book::Fn::Endl << " : " << (std::string)XY << " is out of range in " << Wh.to_string()];
//
//    Cursor = Dc->Peek(Pt);
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::SetColors(Color::Pair pair_)
//{
//    Char.SetColors(pair_);
//
//    return *this;
//}
//
//Book::Result CWindow::Painter::SetupGeometry()
//{
//    //diagnostic::debug(sfnl) < " painter _dc's '" < color::Chartreuse6 <  "Geometry: " <  _dc->geometry();
//    if(!Wh)
//        Wh = Dc->Geometry();
//    else
//        Wh = Dc->Geometry()  & Wh;
//
//    if (!Wh)
//        throw AppBook::Exception()[AppBook::Fatal() << ": " << Book::Fn::Endl << " - Attempt to < Construct > a painter object using invalid Geometry : " << (std::string)Wh];
//
//    //diagnostic::debug(sfnll) < " Configured Geometry:" < color::Yellow < _r < color::Reset < " :";
//    Cursor = Dc->Peek(Wh.A);
//    //diagnostic::debug(SourceLocation) <<  VDC::Cell{ *_cursor }.Details();
//    return Book::Result::Accepted;
//}
//
//CWindow::Painter &CWindow::Painter::Clear()
//{
//    AppBook::Debug() << (std::string)Wh;
//    auto* CC = Cursor;
//    Cursor = Dc->Peek(Wh.A);
//    Char << ' ';
//    Char.SetFg(CWindow::Char(DefAttr).Fg());
//
//    for(int y = 0; y< Wh.Height(); y++)
//    {
//        GotoXY({ 0,y });
//        for (int X = 0; X < Wh.Width(); X++) *Cursor++ = Char.Mem;
//    }
//
//    Cursor = CC;
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::SetBits(Justify::Type aBits)
//{
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::SetHcenter(bool S)
//{
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::SetVcenter(bool S)
//{
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::SetCenter(bool S)
//{
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::SetWordWrap(bool S)
//{
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::SetBg(Color::Code aBg)
//{
//    return *this;
//}
//
//CWindow::Painter &CWindow::Painter::SetFg(Color::Code aFg)
//{
//    return *this;
//}
//
//
//
///*!
//    @brief Computes coordinates within the painter's bloc from the linear index (cursor) location.
//
//    @param none
//    @return computed point.
//    @note none
//    @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
//*/
//Point CWindow::Painter::Position() const
//{
//    auto* B = Dc->MemoryBloc;
//    int dx = static_cast<int>(Cursor-B);
//    return { dx % Dc->Width(), dx / Dc->Width() };
//}
//
//CWindow::Painter &CWindow::Painter::NextChar()
//{
//    auto Pt = Position();
//    Pt.X++;
//    if(!Wh[Pt])
//    {
//
//    }
//    return *this;
//}
//
#pragma endregion CWindow
//


CWindow::CWindow(Util::Object *ParentObj, const std::string &Id) : Object(ParentObj, Id){}

CWindow::~CWindow()
{
    for(auto L: Buffer)
        L.clear();
    Buffer.clear();
}



Book::Result CWindow::Alloc()
{
    if(!R) return Book::Result::Empty;
    Buffer.resize(R.Height());
    for(auto V: Buffer)
    {
        V.resize(R.Width());
        for(auto& C: V)
            C = A;
    }
    return Result::Ok;
}

void CWindow::Clear()
{
    for(auto &L : Buffer)
        for(auto& C: L)
            C = (A & ~(Char::CMask | Char::CharMask)) | 0x20;
}

void CWindow::SetGeometry(Dim Wh)
{
    R = {Point{0,0},Dim{Wh.W,Wh.H}};
    Alloc();
}


int CWindow::Width()
{
    return R.Width();
}

int CWindow::Height()
{
    return R.Height();
}

std::vector<CWindow::Char> &CWindow::operator[](size_t Line)
{
    if(Line>=Buffer.size())
    {
        AppBook::Exception() [ Book::Except() << " Index out of boundaries" ];
    }
    return Buffer[Line];

}

Book::Result CWindow::ReAlloc()
{
    return Alloc();
}













#pragma region CWindowPencil

int CWindow::Pencil::Width()
{
    return 0;
}

int CWindow::Pencil::Height()
{
    return 0;
}

CWindow::Pencil::Pencil(CWindow *W, CWindow::Char::Type DefaultAttr, Rect Sub)
{

}

CWindow::Pencil::~Pencil()
{

}

CWindow::Pencil &CWindow::Pencil::operator<<(const std::string &Input)
{
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(CWindow::Char C)
{
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(char C)
{
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(Color::Code C)
{
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(Color::Pair Colors)
{
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(Utf::Glyph::T Ic)
{
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(Utf::AccentFR::T Ac)
{
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(Utf::Cadre::Index If)
{
    return *this;
}

Point CWindow::Pencil::Position(Point XY)
{
    return Point();
}

void CWindow::Pencil::Clear(CWindow::Char::Type A)
{

}

Point CWindow::Pencil::operator++()
{
    return Point();
}

Point CWindow::Pencil::operator++(int)
{
    return Point();
}

Point CWindow::Pencil::operator--()
{
    return Point();
}

Point CWindow::Pencil::operator--(int)
{
    return Point();
}

#pragma endregion CWindowPencil


} // Book::ConIO

