//
// Created by oldlonecoder on 24-04-12.
//

//#ifndef APPBOOK_SCREEN_H
//#define APPBOOK_SCREEN_H

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
#include <AppBook/ConsoleUI/UiObject.h>


namespace Book::ConsoleUI
{


class APPBOOK_EXPORTS Screen
{
    Rect        mR{};
    Color::Pair mCharColor;


public:

    static constexpr uint32_t EraseChar{0x00FFFF20};
    //...

    struct APPBOOK_EXPORTS Char
    {
        using Type     = uint32_t;
        using Ptr      = uint32_t*;

        Type  M{Screen::EraseChar};

        static constexpr uint32_t CharMask	= 0x000000FF;
        static constexpr uint32_t FGMask	= 0x0000FF00;
        static constexpr uint32_t BGMask	= 0x00FF0000;
        static constexpr uint32_t ColorMask	= 0x00FFFF00;
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

    };

};

} // Book::ConsoleUI

//#endif //APPBOOK_SCREEN_H
