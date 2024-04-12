//
// Created by oldlonecoder on 24-04-11.
//


#include <AppBook/Console/CWindow.h>


namespace Book::ConIO
{


#pragma region CWindowPencil

int CWindow::Pencil::Width() const
{
    return R.Width();
}

int CWindow::Pencil::Height() const
{
    return R.Height();
}

CWindow::Pencil::Pencil(CWindow *W, CWindow::Char::Type DefaultAttr, Rect Sub):
    Window(W), A(DefaultAttr), R(Sub){}


/*!
 * @brief
 * @param C
 * @return
 */
CWindow::Pencil &CWindow::Pencil::operator<<(CWindow::Char C)
{
    *Window->Peek(R) = C.Mem;
    ++R;
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(char C)
{
    A = *Window->Peek(R) = (A & ~CWindow::Char::UTFMASK) | C;
    ++R;
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
    A = *Window->Peek(R) = (A & ~CWindow::Char::FGMask) | (C << CWindow::Char::FGShift);
    return *this;
}


CWindow::Pencil &CWindow::Pencil::operator<<(Color::Pair Colors)
{
    A = *Window->Peek(R) = (A & ~CWindow::Char::CMask) | (Colors.Bg << CWindow::Char::BGShift) | (Colors.Fg << CWindow::Char::FGShift);
    return *this;
}


CWindow::Pencil &CWindow::Pencil::operator<<(Utf::Glyph::Type Ic)
{
    Book::Debug() << " Current position :" << R.Cursor;
    A = *Window->Peek(R) = (A & ~(CWindow::Char::UTFMASK | CWindow::Char::AttrMask)) | CWindow::Char::UGlyph | Ic;
    A &= ~(CWindow::Char::UGlyph);
    ++R;
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(Utf::AccentFR::Type Ac)
{
    A = *Window->Peek(R) = (A & ~CWindow::Char::UTFMASK) | CWindow::Char::Accent | Ac;
    ++R;
    return *this;
}

CWindow::Pencil &CWindow::Pencil::operator<<(Utf::Cadre::Index If)
{
    A = *Window->Peek(R) = (A & ~(CWindow::Char::AttrMask | CWindow::Char::UTFMASK)) | CWindow::Char::Frame | If;
    ++R;
    return *this;
}


Book::Result CWindow::Pencil::Position(Point XY)
{
    if(!R.GotoXY(XY))
    {
        throw AppBook::Exception()[Book::Except() << " Out of bounds :" << XY];
        return Book::Result::Rejected;
    }
    return Book::Result::Accepted;
}

void CWindow::Pencil::Clear(CWindow::Char::Type Attr)
{
    A = Attr;
    R.Home();
    CWindow::Type P = Window->Peek(R);
    do *P++ = Attr; while(R++);
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
    int Delta = std::min(static_cast<int>(Input.length()), (R.Width() - R.Cursor.X));

    if (Delta <= 0) {
        Book::Error() << "Out of boundaries or ";
        return *this;
    }
    auto Src = Input.begin();
    int Y = R.Cursor.Y;
    auto* PChar = Window->Peek(R);
    do
    {
        *PChar++ = (A & ~CWindow::Char::UTFMASK) | *Src++; // Wraps at Col=0
        ++R;
        if(Y != R.Cursor.Y)
        {
            PChar = Window->Peek(R); // At least Wrap to the beginning column at R.A if it is not {0,Y}
            Y = R.Cursor.Y;
        }
    }while(Src != Input.end()); // For now fill in with the entire string.

    return *this;
}




/*!
 * @brief  Prefix increment
 *
 * @note Increments X axe.
 * @return bool
 */
bool CWindow::Pencil::operator++()
{
    return ++R;
}


/*!
 * @brief  Postfix increment
 *
 * @note Increments Y axe.
 * @return bool
 */
bool CWindow::Pencil::operator++(int)
{
    return ++R;
}

/*!
 * @brief  Prefix decrement
 *
 * @note Decrements X axe.
 * @return bool
 */
bool CWindow::Pencil::operator--()
{
    return --R;
}


/*!
 * @brief  Postfix decrement
 *
 * @note Decrements X axe.
 * @return bool
 */
bool CWindow::Pencil::operator--(int)
{
    return --R;
}

void CWindow::Pencil::Clear(Color::Code C)
{
    A &= ~CWindow::Char::UTFMASK | 0x20;
    A |= CWindow::Char{A}.SetBg(C).Mem;
    R.Home();
    CWindow::Type P = Window->Peek(R);
    do *P++ = A; while(R++);
}

#pragma endregion CWindowPencil

}