//
// Created by oldlonecoder on 24-04-12.
//

//#ifndef APPBOOK_CHAR_H
//#define APPBOOK_CHAR_H

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

#pragma once
#include <AppBook/ConIO/UiEnums.h>
#include <AppBook/Utf/Cadres.h>


namespace Book::Ui
{


struct APPBOOK_API Char
{
    using Type = uint32_t;
    using Ptr  = Char*;
    using Bloc = Char*;

    Type  M{0x00000020}; ///< Will put default value somewhere else eventually...


    //static constexpr Type ClearScreen{0x00FFFF20};


    static constexpr uint32_t Mask	    = 0x000000FF;
    static constexpr uint32_t FGMask	= 0x0000FF00;
    static constexpr uint32_t BGMask	= 0x00FF0000;
    static constexpr uint32_t BgFgMask  = 0x00FFFF00;
    static constexpr uint32_t AttrMask	= 0xFF000000;
    static constexpr uint32_t UtfMask	= 0xF10000FF;
    // ==================Attributes==========================
    static constexpr uint32_t UGlyph 	= 0x01000000;
    static constexpr uint32_t Underline = 0x02000000;
    static constexpr uint32_t Stroke 	= 0x04000000;
    static constexpr uint32_t Blink 	= 0x08000000;
    static constexpr uint32_t Accent 	= 0x10000000;
    static constexpr uint32_t Frame 	= 0x20000000;

    // ==============Bit Shift ============================
    static constexpr int FGShift = 0x08;
    static constexpr int BGShift = 0x10;
    static constexpr int ATShift = 0x18;

    Char() = default;
    ~Char() = default;

    Char(Char::Type Bits);
    Char(char C);
    Char(Char* Pc);

    Char& operator = (Char::Type CBits);
    Char& operator = (char C);
    Char& operator = (Char::Type* CBits);
    Char& operator = (const Char&) = default;
    Char& operator = (Char&&) noexcept = default;


    Char& ClearAttributes();
    Char& ClearUtf();
    Char& ClearFg();
    Char& ClearBg();
    Char& ClearBgFg();
    Char& ClearChar();

    Char& SetAccentFR(Utf::AccentFR::Type A);
    Char& SetGlyph(Utf::Glyph::Type G);
    Char& SetFg(Color::Code F);
    Char& SetBg(Color::Code B);
    Char& SetBgFg(Color::Pair Cp);
    Char& SetUnderLine(bool U=true);
    Char& SetBlink(bool B=true);



    Char& Clear();
    Char& operator[](Char::Type ClrBits);
    Char& operator|(Char::Type C);
    Char& operator|(Color::Code C);
    Char& operator|(Color::Pair Cp);
    Char& operator|(Utf::Glyph::Type Ic);
    Char& operator|(Utf::AccentFR::Type Ac);

    Color::Code BackgroundColor() const;
    Color::Code ForegroundColor() const;
    std::pair<bool, Utf::Glyph::T>  Graphen() const;
    std::pair<bool, std::string> FrameChar() const;
    std::pair<bool, std::string> AccentFlag() const;
    Color::Pair  Colors(Char::Type E);

};

} // Book::ConIO

//#endif //APPBOOK_CHAR_H
