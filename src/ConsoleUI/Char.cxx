//
// Created by oldlonecoder on 24-04-12.
//

#include "AppBook/ConsoleUI/Char.h"

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




namespace Book::ConsoleUI
{


Char& Char::ClearAttributes()
{
    M &= ~AttrMask;
    return *this;
}

Char& Char::ClearUtf()
{
    M &= ~UtfMask;
    return *this;
}

Char& Char::ClearFg()
{
    M &= ~FGMask;
    return *this;
}

Char& Char::ClearBg()
{
    M &= ~BGMask;
    return *this;
}

Char& Char::ClearBgFg()
{
    M &= ~BgFgMask;
    return *this;
}

Char& Char::ClearChar()
{
    M &= ~Mask;
    return *this;
}

Char &Char::Clear()
{
    return *this;
}



/*!
 * @brief Convenient function to execute the clear bits:
 * @param ClrBits
 * @return Ref-To-Self
 *
 * @code
 * Ex.:
 *      //...
 *      Clear()[Char::UtfMask | BgFgMask ] | Color::Yellow | 0x20;
 *
 */
Char &Char::operator[](Char::Type ClrBits)
{
    M &= ~ClrBits;
    return *this;
}

Char &Char::operator|(Char::Type C)
{
    M |= C;
    return *this;
}

Char &Char::operator|(Color::Code C)
{
    return ClearFg() | static_cast<Char::Type>(C<<FGShift);
}


Char &Char::operator|(Color::Pair Cp)
{
    return ClearBgFg() | static_cast<Char::Type>(Cp.Bg << BGShift) | static_cast<Char::Type>(Cp.Fg << FGShift);
    //return *this;
}


Char &Char::operator|(Utf::Glyph::Type Ic)
{
    return SetGlyph(Ic);
}

Char &Char::operator|(Utf::AccentFR::Type Ac)
{
    ClearUtf();
    M |= Accent|Ac;
    return *this;
}

Char &Char::SetAccentFR(Utf::AccentFR::Type A)
{
    ClearUtf();
    M|= Char::Accent | A;
    return *this;
}

Char &Char::SetGlyph(Utf::Glyph::Type G)
{
    ClearUtf();
    M|= Char::UGlyph| G;
    return *this;
}

Char &Char::SetFg(Color::Code F)
{
    ClearFg();
    M|=  F << FGShift;
    return *this;
}

Char &Char::SetBg(Color::Code B)
{
    ClearBg();
    M|=  B << BGShift;
}

Char &Char::SetBgFg(Color::Pair Cp)
{
    return ClearBgFg().SetBg(Cp.Bg).SetFg(Cp.Fg);

}

Char::Char(Char::Type Bits): M(Bits){};


Char::Char(char C): M(C){}

Char::Char(Char *Pc):M(Pc->M){}


Char &Char::operator=(Char::Type CBits)
{
    M = CBits;
    return *this;
}

Char &Char::operator=(char C)
{
    ClearUtf();
    M |= C;
    return *this;
}

Char &Char::operator=(Char::Type *CBits)
{
    M = *CBits;
    return *this;
}

Color::Code Char::BackgroundColor() const
{
    return static_cast<Color::Code>((M & BGMask) >> BGShift);
}

Color::Code Char::ForegroundColor() const
{
    return static_cast<Color::Code>((M & FGMask) >> FGShift);
}

std::pair<bool, Utf::Glyph::T> Char::Graphen() const
{
    if(M & UGlyph) return {true, Utf::Glyph::Data[static_cast<Utf::Glyph::Type>(M&0xFF)]};
    return {};
}

std::pair<bool, std::string> Char::FrameChar() const
{
    if(M & Frame) return {true, Utf::Cadre()[static_cast<Utf::Cadre::Index>(M&0xFF)]};
    return {};
}

std::pair<bool, std::string> Char::AccentFlag() const
{
    if(M & Char::Accent) return {true, Utf::AccentFR::Data[static_cast<Utf::AccentFR::Type>(M&0xFF)]};
    return {};
}



} // Book::ConsoleUI

