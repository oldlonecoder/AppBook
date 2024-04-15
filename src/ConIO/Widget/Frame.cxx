//
// Created by oldlonecoder on 24-04-15.
//

#include "AppBook/ConIO/Widget/Frame.h"

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


Frame::Frame()
{}




Frame::Frame(Util::Object *parentObj, const std::string &uid, WClass::Type cc) : UiElement(parentObj, uid, cc)
{
    Attr = Char(Attr).SetBgFg(AttrDB::DB["Frame"][State::Active]).M;
}

Frame::Frame(UiElement *parentObj, const std::string &uid, WClass::Type cc) : UiElement(parentObj, uid, cc)
{
    Attr = Char(Attr).SetBgFg(AttrDB::DB["Frame"][State::Active]).M;
}

Frame::~Frame()
{
}

void Frame::Show()
{
    UiElement::Show();
}
} // Book::Ui
