//
// Created by oldlonecoder on 24-04-11.
//


#include <AppBook/Console/Window.h>


namespace Book::ConIO
{


#pragma region CWindow_Char


Window::Char::Char(Window::Char::Type _Ch): Mem(_Ch){}
Window::Char::Char(Window::Char* _C): Mem(_C->Mem){}

Window::Char &Window::Char::SetFg(Color::Code fg_)
{
    Mem = (Mem & ~FGMask) | (static_cast<Char::Type>(fg_) << FGShift);
    return *this;
}

Window::Char &Window::Char::SetBg(Color::Code bg_)
{
    Mem = (Mem & ~BGMask) | (static_cast<Char::Type>(bg_) << BGShift);
    return *this;
}

[[maybe_unused]] Window::Char &Window::Char::SetAttributes(Window::Char::Type d_)
{
    Mem = (Mem & ~AttrMask) | d_;
    return *this;
}


[[maybe_unused]] Window::Char &Window::Char::SetColors(Color::Pair &&c_)
{
    return SetFg(c_.Fg).SetBg(c_.Bg);
}

[[maybe_unused]] Window::Char &Window::Char::SetColors(Color::Pair &c_)
{
    return SetFg(c_.Fg).SetBg(c_.Bg);
}

Window::Char &Window::Char::operator=(Window::Char::Type d_)
{
    Mem = d_;
    return *this;
}


Window::Char &Window::Char::operator=(char d_)
{
    Mem = (Mem & ~CharMask) | d_;
    return *this;
}

Color::Code Window::Char::Fg() const
{
    return static_cast<Color::Code>((Mem & FGMask) >> FGShift);
}

Color::Code Window::Char::Bg() const
{
    return static_cast<Color::Code>((Mem & BGMask) >> BGShift);
}

[[maybe_unused]] Color::Pair Window::Char::Colors() const
{
    return {Fg(),Bg()};
}

[[maybe_unused]] Utf::Glyph::Type Window::Char::IconID() const
{
    if (!(Mem & Char::UGlyph)) return Utf::Glyph::Poop;
    auto Ic = Mem & Char::CharMask;
    if (Ic > 103) Ic = Utf::Glyph::Bug;
    return  Ic;
}

[[maybe_unused]] Utf::AccentFR::Type Window::Char::AccentID() const
{
    auto AID = Mem & Char::CharMask;
    if (AID > Utf::AccentFR::Ucirc) return Utf::AccentFR::Agrave;
    return static_cast<Utf::AccentFR::Type>(AID);
}

uint8_t Window::Char::Ascii() const
{
    return Mem & CharMask;
}

[[maybe_unused]] uint16_t Window::Char::Attributes() const
{
    return (Mem & AttrMask) >> ATShift;
}

Window::Char &Window::Char::operator<<(Utf::Glyph::Type i_)
{
    Mem = (Mem & ~CharMask) | i_ | UGlyph;
    return *this;
}

Window::Char &Window::Char::operator<<(Utf::AccentFR::Type a_)
{
    Mem = (Mem & ~CharMask) | a_ | Accent;
    return *this;
}

[[maybe_unused]] std::string Window::Char::RenderColors() const
{
    std::string str;
    str += Color::Ansi({Fg(),Bg()});
    return str;
}

[[maybe_unused]] Window::Char &Window::Char::ResetAttributes(Window::Char::Type)
{
    Book::Status() << " This feature is not determined in this context, then not implemented yet";
    return *this;
}


Window::Char::operator std::string() const
{
    return Details();
}

std::string Window::Char::Details() const
{
    StrAcc Str = "%s[Char:'%s%d%s'/Fg:'%s%s%s'/Bg:'%s%s%s']";
    Str << Color::White <<Color::Yellow << (int)Ascii() << Color::White
        << Color::Yellow << Color::Name(Fg()) << Color::White
        << Color::Yellow << Color::Name(Bg()) << Color::White;

    return Str();
}


#pragma endregion CWindow_Char



}