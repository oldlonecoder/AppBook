//
// Created by oldlonecoder on 24-04-13.
//

#include "AppBook/ConIO/Widget/Icon.h"

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


Icon::Icon(Util::Object *ParentObj): Element(ParentObj, Utf::Glyph::Data[Utf::Glyph::Debian], WClass::Glyph)
{
    std::string strid = "{ " + Id() +" }" + " Glyph Group";
    SetId(strid);
    SetGeometry({1,1});
    // Override Element's ColorPair
    Attr = Char(Attr).SetBgFg(AttrDB::DB["Glyph"][State::Active]).M;
}

Icon &Icon::operator=(Utf::Glyph::Type IIc)
{
    Ic = IIc;
    return *this;
}

std::string Icon::operator()()
{
    return Utf::Glyph::Data[Ic];
}

Result Icon::Render(Rect SubR)
{
    Debug();
    if(auto [b,IcStr] = Bloc[0].Graphen(); b)
    {
        Console::GotoXY(GetScreenXY());
        Console::SetColors(Attr);
        Console::Write(IcStr,true);
    }
    else
        return Result::Rejected;
    return Result::Done;
}

void Icon::Show()
{
//    auto Cp = Char(Attr).Colors();
//    Debug() << " Colors: {" << Color::Name(Cp.Fg) << ", " << Color::Name(Cp.Bg) << "}";

    Clear();

    Bloc->SetGlyph(Ic);
}


} // Book::Ui
