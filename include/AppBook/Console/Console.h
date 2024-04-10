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
#include <AppBook/Console/CWindow.h>


namespace Book::ConIO
{

class CWindow;

struct APPBOOK_EXPORTS Console
{

    Dim     Wh;
    Point   Cursor{0,0};
    static Console* Terminal;

    Console();

    Book::Result GotoXY(const Point& XY);
    size_t Write(const std::string& Text);

    //...
    static void Render(CWindow* W, Rect SubR);

    static void GetGeometry();

    Console& operator << (const Color::Code& C);
    Console& operator << (const Color::Pair& BgFg);
    Console& operator << (const CWindow::Char& Char);
    Console& operator << (const char* Text);
    Console& operator << (const char& C);

    //Console& operator << (CWindow::Char::Type Char32);




};

} // Book::ConIO

//#endif //APPBOOK_CONSOLE_H
