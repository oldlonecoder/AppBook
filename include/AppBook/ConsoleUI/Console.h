//
// Created by oldlonecoder on 24-04-10.
//

//#ifndef APPBOOK_CONSOLE_H
//#define APPBOOK_CONSOLE_H

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
#include <AppBook/Console/Window.h>


namespace Book::ConsoleUI
{

class Window;

struct APPBOOK_EXPORTS Console
{

    Dim     Wh;
    Point   Cursor{0,0};

    Console()=default;
    ~Console() = default;

    Book::Result GotoXY(const Point& XY);
    size_t Write(const std::string& Text);
    size_t Write(const char* Text);
    size_t Write(const char& Char8);
    size_t DrawIcon(Utf::Glyph::Type IcID);
    void Render(Window* W, Rect SubR);
    void GetGeometry();
    void SetFgColor(Color::Code Code);
    void SetBgColor(Color::Code Code);
    void SetColor(Color::Pair BgFg);





};

} // Book::ConsoleUI

//#endif //APPBOOK_CONSOLE_H
