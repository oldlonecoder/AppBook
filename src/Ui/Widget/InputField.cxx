//
// Created by oldlonecoder on 24-04-18.
//

#include "AppBook/Ui/Widget/InputField.h"

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


InputField::~InputField()
{

}

InputField::InputField(Element *ParentEl, const std::string &Uid):Element(ParentEl, Uid),
PlaceHolder(Uid){}

void InputField::SetGeometry(const Dim& Geo)
{
    Element::SetGeometry(Geo);
    if(Geo.H > 1)
    {
        // Icon on line 0
        // Set frame or underline explicitly  on every draw on empty lines.
        ;
    }
}

Book::Result InputField::Render(Rect SubR)
{
    Attr |= Char::Underline;
    Element::Render({});
    //...
    R.Home();
    return Result::Done;
}

}//namespace Book::Ui