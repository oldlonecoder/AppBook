//
// Created by oldlonecoder on 24-04-13.
//

#include "Icon.h"

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




namespace Book::Ui
{


Icon::Icon(Util::Object *ParentObj): UiElement(ParentObj, Utf::Glyph::Data[Utf::Glyph::Alien], WClass::Glyph){}

Icon &Icon::operator=(Utf::Glyph::Type IIc)
{
    Ic = IIc;
    return *this;
}

std::string Icon::operator()()
{
    return Utf::Glyph::Data[Ic];
}

} // Book::Ui
