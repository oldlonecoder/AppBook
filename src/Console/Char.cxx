//
// Created by oldlonecoder on 24-04-11.
//


#include <AppBook/Console/CWindow.h>


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



}