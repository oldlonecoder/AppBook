//
// Created by oldlonecoder on 24-04-11.
//


#include <AppBook/Console/Window.h>


namespace Book::ConIO
{


#pragma region CWindowPencil

int Window::Pencil::Width() const
{
    return R.Width();
}

int Window::Pencil::Height() const
{
    return R.Height();
}

Window::Pencil::Pencil(Window *W, Window::Char::Type DefaultAttr, Rect Sub):
    Window(W), A(DefaultAttr), R(Sub){}


/*!
 * @brief
 * @param C
 * @return
 */
Window::Pencil &Window::Pencil::operator<<(Window::Char C)
{
    *Window->Peek(R) = C.Mem;
    ++R;
    return *this;
}

Window::Pencil &Window::Pencil::operator<<(char C)
{
    A = *Window->Peek(R) = (A & ~Window::Char::UTFMASK) | C;
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
Window::Pencil &Window::Pencil::operator<<(Color::Code C)
{
    A = *Window->Peek(R) = (A & ~Window::Char::FGMask) | (C << Window::Char::FGShift);
    return *this;
}


Window::Pencil &Window::Pencil::operator<<(Color::Pair Colors)
{
    A = *Window->Peek(R) = (A & ~Window::Char::CMask) | (Colors.Bg << Window::Char::BGShift) | (Colors.Fg << Window::Char::FGShift);
    return *this;
}


Window::Pencil &Window::Pencil::operator<<(Utf::Glyph::Type Ic)
{
    Book::Debug() << " Current position :" << R.Cursor;
    A = *Window->Peek(R) = (A & ~(Window::Char::UTFMASK | Window::Char::AttrMask)) | Window::Char::UGlyph | Ic;
    A &= ~(Window::Char::UGlyph);
    ++R;
    return *this;
}

Window::Pencil &Window::Pencil::operator<<(Utf::AccentFR::Type Ac)
{
    A = *Window->Peek(R) = (A & ~Window::Char::UTFMASK) | Window::Char::Accent | Ac;
    ++R;
    return *this;
}

Window::Pencil &Window::Pencil::operator<<(Utf::Cadre::Index If)
{
    A = *Window->Peek(R) = (A & ~(Window::Char::AttrMask | Window::Char::UTFMASK)) | Window::Char::Frame | If;
    ++R;
    return *this;
}


Book::Result Window::Pencil::Position(Point XY)
{
    if(!R.GotoXY(XY))
    {
        throw AppBook::Exception()[Book::Except() << " Out of bounds :" << XY];
        return Book::Result::Rejected;
    }
    return Book::Result::Accepted;
}

void Window::Pencil::Clear(Window::Char::Type Attr)
{
    A = Attr;
    R.Home();
    Window::Type P = Window->Peek(R);
    do *P++ = Attr; while(R++);
}



/*!
 * @brief Write a std::string contents onto the Window's internal Buffer at the current inner Cursor position.
 *
 * <li>Content must be non-formatted string because the length cannot be calculated from this method then surely the pre-formatted length is invalid.</li>
 * <li>Write length is cut at the end of the current line in the buffer.</li>
 * <li> (for now) Inner CursorXY is advanced but only on the current line, so Position({X,Y}) must be manually set for each line
 * @param Input
 * @return Ref-To-Self
 * @note Write will be prevented or stop as soon the CursorXY reaches the end of the current line.
 */
Window::Pencil &Window::Pencil::operator<<(const std::string &Input)
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
        *PChar++ = (A & ~Window::Char::UTFMASK) | *Src++; // Wraps at Col=0
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
bool Window::Pencil::operator++()
{
    return ++R;
}


/*!
 * @brief  Postfix increment
 *
 * @note Increments Y axe.
 * @return bool
 */
bool Window::Pencil::operator++(int)
{
    return ++R;
}

/*!
 * @brief  Prefix decrement
 *
 * @note Decrements X axe.
 * @return bool
 */
bool Window::Pencil::operator--()
{
    return --R;
}


/*!
 * @brief  Postfix decrement
 *
 * @note Decrements X axe.
 * @return bool
 */
bool Window::Pencil::operator--(int)
{
    return --R;
}

void Window::Pencil::Clear(Color::Code C)
{
    A &= ~Window::Char::UTFMASK | 0x20;
    A |= Window::Char{A}.SetBg(C).Mem;
    R.Home();
    Window::Type P = Window->Peek(R);
    do *P++ = A; while(R++);
}

void Window::Pencil::ClearBg(Color::Code C)
{

}

#pragma endregion CWindowPencil

}