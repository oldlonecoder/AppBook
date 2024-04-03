//
// Created by oldlonecoder on 24-04-02.
//

//#ifndef APPBOOK_CADRES_H
//#define APPBOOK_CADRES_H
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

#include <AppBook/Util/StrAcc.h>
#include "AppBook/Util/Geometry.h"

namespace Utf
{


struct APPBOOK_EXPORTS Cadre
{
    Color::Pair Colors;     ///< Colours of the Box::Frame
    Rect R;          ///< Box ...geometry

/*!
 * @brief 1v x 4h matrix for addressing Frame components
 * @note  : Source : https://github.com/ArthurSonzogni/FTXUI/blob/main/src/ftxui/screen/string.cpp
 */
    struct APPBOOK_EXPORTS FrameMat
    {
        uint8_t Left : 2;
        uint8_t Top  : 2;
        uint8_t Right: 2;
        uint8_t Down : 2;
        uint8_t Round: 1;

        bool operator < (Cadre::FrameMat RHS) const;
    };

    struct APPBOOK_EXPORTS FrameData
    {
        Utf::Glyph::T TL; ///< Top Left
        Utf::Glyph::T TR; ///< Top Right
        Utf::Glyph::T BL; ///< Bottom Left
        Utf::Glyph::T BR; ///< Bottom Right
        Utf::Glyph::T HL; ///< Horizontal Line
        Utf::Glyph::T VL; ///< Vertical Line
        Utf::Glyph::T CVH;///< Cross Vertical-Horizontal
        Utf::Glyph::T CRL;///< Cross Right-towards-Left
        Utf::Glyph::T CLR;///< Cross Left-towards-Right
        Utf::Glyph::T CTD;///< Cross Top-towards-Down
        Utf::Glyph::T CDT;///< Cross Down-towards-Top
        Cadre::FrameData& operator = (Cadre::FrameMat FrameModel);
    }Frame{ "┏", "┓", "┗", "┛", "━", "┃", "╋", "┫", "┣", "┳", "┻" }; ///< By ...default, hardcode the ...default model.
/*           ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━{x,x,x,0,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━{x,0,x,x,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━{0,x,x,x,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━{x,x,0,x,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━{x,x,x,x,0}
             ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━{0,x,0,x,0}
             ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{x,0,x,0,0}
             ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{x,x,0,0,0}
             ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{0,x,x,0,0}
             ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{x,0,0,x,0}
             ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{0,0,x,x,0}

*/

    Cadre::FrameMat Model{2,2,2,2,0}; ///< (default) Frame model to apply to FrameData.
    static std::map<std::string_view, Cadre::FrameMat> FrameMatrix;
};


}
//#endif //APPBOOK_CADRES_H
