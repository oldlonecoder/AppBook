//
// Created by oldlonecoder on 24-04-09.
//

//#ifndef APPBOOK_UIENUMS_H
//#define APPBOOK_UIENUMS_H
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

#include <AppBook/Book/AppBook.h>

namespace Book::Ui
{
namespace WClass
{
using Type                     = uint16_t;
static constexpr Type TopLevel = 0x0001;
static constexpr Type Floating = 0x0002;
static constexpr Type Child    = 0x0004;
static constexpr Type Parent   = 0x0008;
static constexpr Type Caption  = 0x0010;
static constexpr Type Frame    = 0x0020;
static constexpr Type Input    = 0x0040;
static constexpr Type Element  = 0x0080; ///< Intentional always ON bit.
static constexpr Type Glyph    = 0x0100; ///< Intentional always ON bit.

//...
}
/**
 * @brief Widget ui state enums or'ed flags
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace State
{
using Type                    = uint16_t;
static constexpr Type Normal  = 0x00;
static constexpr Type Disable = 0x01;
static constexpr Type Active  = 0x02;
static constexpr Type Focus   = 0x04;
static constexpr Type Error   = 0x08;
static constexpr Type Warning = 0x10;
static constexpr Type Illegal = 0x20;
static constexpr Type Invalid = 0x40;
static constexpr Type Visible = 0x80;
static constexpr Type Success = 0x100;
static constexpr Type Selected= 0x200;

}



} // Book::Ui

//#endif //APPBOOK_UIENUMS_H