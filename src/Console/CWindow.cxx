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

[[maybe_unused]] CWindow::Char &CWindow::Char::ResetAttributes(CWindow::Char::Type)
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


#pragma endregion CWindow_Char



#pragma region CWindow

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

CWindow::Pencil &CWindow::BeginWrite(Rect Geom, CWindow::Char::Type Attr)
{
    return *(new Pencil(this, Attr, Geom));
}

Book::Result CWindow::EndWrite(CWindow::Pencil &Pen)
{
    delete &Pen;
    return Result::Ok;
}

Book::Result CWindow::operator>>(StrAcc &Acc)
{
//    vdc::type p = mem_->peek(xy_);
//    w_ = w_ == 0 ? mem_->width() - xy_.x
//                 : w_ > mem_->width() - xy_.x ? mem_->width() - xy_.x : w_;
//    //w_ =
//    vdc::cell cell=p;
//    vdc::cell prev_cell=p;
//    using ansi = tux::attr<textattr::format::ansi256>;
//    for(int x = 0; x< w_; x++)
//    {
//        if (prev_cell.bg() != cell.bg()) (*terminal) << ansi::bg(cell.bg());
//        if (prev_cell.fg() != cell.fg()) (*terminal) << ansi::fg(cell.fg());
//        if (cell.mem & vdc::cell::UGlyph)
//        {
//            auto Ic =  cell.icon_id();
//            write(1, Icon::Data[Ic], std::strlen(Icon::Data[Ic]));
//        }
//        else
//            write(1,&cell.mem,1);
//
//        prev_cell = p++;
//        cell = p;
//    }

    CWindow::Char PrevCell = Buffer[0][0];

    for(int Y=0; Y<R.Height(); Y++)
    {
        PrevCell = Buffer[Y][0];
        for(int X = 0; X< R.Width(); X++)
        {
            auto D = Buffer[Y][X];
            if(D.Mem & CWindow::Char::Frame)
            {
                Utf::Cadre Cadre;
                Acc << *Cadre.Data[static_cast<int>(D.Mem & CWindow::Char::CharMask)];
                PrevCell = D;
                continue; // Colors or any other attributes changes on Frame is bypassed (rejected).
            }
            if(D.Bg() != PrevCell.Bg())         Acc << Color::AnsiBg(D.Bg());
            if(D.Fg() != PrevCell.Fg())         Acc << Color::Ansi(D.Fg());
            if(D.Mem & CWindow::Char::UGlyph)
            {
                Acc << D.IconID();
            }
            else // Can't have an ASCII character -> and -> a Glyph at the same position.
                Acc << static_cast<char>(D.Mem & CWindow::Char::CharMask);
            PrevCell = D;
        }
        Acc << '\n'; ///@note  Ambiguous Assignation because This 2D text would be written also in logging journal as in this AppBook API/Framework
    }
    return Result::Ok;
}


#pragma endregion CWindow



#pragma region CWindowPencil

int CWindow::Pencil::Width() const
{
    return R.Width();
}

int CWindow::Pencil::Height() const
{
    return R.Height();
}

CWindow::Pencil::Pencil(CWindow *W, CWindow::Char::Type DefaultAttr, Rect Sub):Util::Object(W, "CWindow::Pencil"),
Window(W), A(DefaultAttr), R(Sub){}

//CWindow::Pencil::~Pencil(){}






#define CHECKOOB if(CursorXY.X >= R.Width()) \
{                                            \
   Book::Error() << " Out of boundaries";    \
   return *this;\
}
/*!
 * @brief Write a std::string contents onto the CWindow's internal Buffer at the current inner Cursor position.
 *
 * <li>Content must be non-formatted string because the length cannot be calculated from this method then surely the pre-formatted length is invalid.</li>
 * <li>Write length is cut at the end of the current line in the buffer.</li>
 * <li> (for now) Inner CursorXY is advanced but only on the current line, so Position({X,Y}) must be manually set for each line
 * @param Input
 * @return Ref-To-Self
 * @note Write will be prevented or stop as soon the CursorXY reaches the end of the current line.
 */
CWindow::Pencil &CWindow::Pencil::operator<<(const std::string &Input)
{
    int Delta = std::min(static_cast<int>(Input.length()), (R.Width() - CursorXY.X));

    if(Delta <= 0)
    {
        Book::Error() << "Out of boundaries or ";
        return *this;
    }
    auto Src = Input.begin();
    auto& Line = Window->Buffer[CursorXY.Y];
    for(int X=CursorXY.X; (X < R.Width()) && (Src != Input.end()); X++, Src++)
    {
        Line[X] = A & ~(CWindow::Char::CharMask) | *Src;
        if(!++(*this)) return *this; // Double check while INC CursorXY.X ( prefix INC )
    }
    return *this;
}


/*!
 * @brief
 * @param C
 * @return
 */
CWindow::Pencil &CWindow::Pencil::operator<<(CWindow::Char C)
{
    CHECKOOB
    Window->Buffer[CursorXY.Y][CursorXY.X] = C;
    ++(*this);
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(char C)
{
    CHECKOOB
    Window->Buffer[CursorXY.Y][CursorXY.X] = (A & ~CWindow::Char::CharMask) | C;
    ++(*this);
    return *this;
}


/*!
 * @brief Set the Foreground Color attribute
 * @param C Color::Code enumeration.
 *
 * @return Ref-To_Self
 * @note As of this stage, only the Foreground is addressed from this particular method.
 */
CWindow::Pencil &CWindow::Pencil::operator<<(Color::Code C)
{
    CHECKOOB
    auto& D = Window->Buffer[CursorXY.Y][CursorXY.X];
    D = ~(D.Mem & CWindow::Char::FGMask) | (C << CWindow::Char::FGShift);
    return *this;
}


CWindow::Pencil &CWindow::Pencil::operator<<(Color::Pair Colors)
{
    CHECKOOB
    auto& D = Window->Buffer[CursorXY.Y][CursorXY.X];
    D = ~(D.Mem & CWindow::Char::CMask) | (Colors.Bg << CWindow::Char::BGShift) | (Colors.Fg << CWindow::Char::FGShift);
    return *this;
}


CWindow::Pencil &CWindow::Pencil::operator<<(Utf::Glyph::Type Ic)
{
    CHECKOOB
    auto& D = Window->Buffer[CursorXY.Y][CursorXY.X];
    D = (D.Mem & ~(CWindow::Char::UTFMASK | CWindow::Char::CharMask)) | CWindow::Char::UGlyph | Ic;
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(Utf::AccentFR::Type Ac)
{
    CHECKOOB
    auto& D = Window->Buffer[CursorXY.Y][CursorXY.X];
    D = (D.Mem & ~CWindow::Char::CharMask) | CWindow::Char::Accent | Ac;
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(Utf::Cadre::Index If)
{
    CHECKOOB
    auto& D = Window->Buffer[CursorXY.Y][CursorXY.X];
    D = (D.Mem & ~(CWindow::Char::AttrMask | CWindow::Char::CharMask)) | CWindow::Char::Frame | If;
    return *this;
}


Point CWindow::Pencil::Position(Point XY)
{
    if(((XY.X < 0) || (XY.X >= R.Width()))|| ((XY.Y < 0) || (XY.Y >= R.Height())))
        AppBook::Exception() [Book::Except() << " Out of bounds..."];

    CursorXY = XY;
    return CursorXY;
}

void CWindow::Pencil::Clear(CWindow::Char::Type Attr)
{
    A = Attr;
    for(auto Y = 0; Y < R.Height(); Y++)
    {
        for(int X = 0; X < R.Width(); X++)
        {
            Window->Buffer[Y+Window->R.A.Y][X+Window->R.A.X].Mem = Attr;
        }
    }
}


/*!
 * @brief  Prefix increment
 *
 * @note Increments X axe.
 * @return bool
 */
bool CWindow::Pencil::operator++()
{
    if(CursorXY.X >= R.Width()) return false;
    ++CursorXY.X;
    return true;
}



/*!
 * @brief  Postfix increment
 *
 * @note Increments Y axe.
 * @return bool
 */
bool CWindow::Pencil::operator++(int)
{
    if(CursorXY.Y >= R.Height()) return false;
    ++CursorXY.Y;
    return true;

}

/*!
 * @brief  Prefix decrement
 *
 * @note Decrements X axe.
 * @return bool
 */
bool CWindow::Pencil::operator--()
{
    if(CursorXY.X < 0) return false;
    --CursorXY.X;
    return true;
}


/*!
 * @brief  Postfix decrement
 *
 * @note Decrements X axe.
 * @return bool
 */
bool CWindow::Pencil::operator--(int)
{
    if(CursorXY.Y < 0) return false;
    --CursorXY.Y;
    return true;
}

#pragma endregion CWindowPencil


} // Book::ConIO

