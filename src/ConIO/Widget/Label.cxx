//
// Created by oldlonecoder on 24-04-14.
//

#include "AppBook/ConIO/Widget/Label.h"

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




Label::~Label()
{
    Text.Clear();
}

Label::Label(Util::Object *ParentObj, const std::string &LblText) : Element(ParentObj, "Ui Label Group"),
Text(LblText)
{

    auto W= Text.Len();
    SetGeometry(Dim(static_cast<int>(W), 1));

}


void Label::Show()
{
    Book::Debug() << Text() ;
    Element::Show();
    GotoXY({0,0});
    WriteStr(Text());

}

//void Label::SetLeftGlyph(Utf::Glyph::Type G, Color::Pair Cp)
//{
//    LeftIcon = new Icon(this);
//    *LeftIcon = G;
//    LeftIcon->SetColors(Cp);
//    auto W= Text.Len();
//    SetGeometry(Dim(static_cast<int>(W), 1));
//    LeftIcon->SetPosition({0,0});
//}

void Label::SetText(const std::string &NewText)
{
    Text = NewText;
    auto W= Text.Len();
    SetGeometry(Dim(static_cast<int>(W), 1));

}

Result Label::Render(Rect SubR)
{
    Element::Render({});
    Book::Debug() << " Check if LeftIcon :";
    //if(LeftIcon)
    //    return LeftIcon->Render({});
    //...
    return Result::Done;
}


} // Book::Ui
