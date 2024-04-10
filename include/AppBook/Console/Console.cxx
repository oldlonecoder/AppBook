//
// Created by oldlonecoder on 24-04-10.
//

#include "Console.h"

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

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#   include <Windows.h>
#   include <stdlib.h>
#   include <io.h>
#   define write _write
#elif defined(__linux__)
#   include <sys/ioctl.h>
#   include <unistd.h>

#endif // Windows/Linux





namespace Book::ConIO
{


Console* Console::Terminal{nullptr};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
Book::Result Console::GotoXY(const Point &XY) // NOLINT(*-make-member-function-const)
{
    if(!Rect({0,0},Wh)[XY])
    {
        Book::Error() << Result::Rejected << " coord '" << (XY + Point(1,1)) << "' is out of console's geometry.";
        return Book::Result::Rejected;
    }

    std::cout << "\033[" << XY.Y + 1 << ';' << XY.X + 1 << 'H';
    fflush(stdout);
    return Result::Accepted;
}




/*!
 * @brief
 * @param Text
 * @return
 * @note As of now (lol) no control on line wrap, no length control. So as is.
 */
size_t Console::Write(const std::string &Text)
{
    int ln = static_cast<int>(Text.length());
    ::write(1, Text.c_str(), ln);
    Cursor += {ln,0};
    return ln;
}

void Console::Render(CWindow *W, Rect /*SubR*/)
{

    Point Crs{W->R.A};

    auto& PrevCell = W->Buffer[0][0];

    for(auto &Line : W->Buffer)
    {
        Console::Terminal->GotoXY(Crs);
        for(auto Col: Line) (*Console::Terminal) << (Col);
        write(1,"\033[0m",4);
        Crs += {0,1};
    }
    fflush(stdout);
}

Console &Console::operator<<(const Color::Code& C)
{
    auto Seq = Color::Ansi(C);
    (void)Write(Seq);
    return *this;
}

Console &Console::operator<<(const Color::Pair& BgFg)
{
    Write(BgFg());
    return *this;
}

Console &Console::operator<<(const CWindow::Char& Char)
{
    static CWindow::Char Cell;
    StrAcc Acc{};
    if(Char.Mem & CWindow::Char::Frame)
    {
        Write(Utf::Cadre()[static_cast<Utf::Cadre::Index>(Char.Mem & CWindow::Char::CharMask)]);
        return *this;
    }
    if(Char.Bg() != Cell.Bg())
        Write(Color::AnsiBg(Char.Bg()));
    if(Char.Fg() != Cell.Fg())
        (*this) << Char.Fg();// Color::Ansi(D.Fg());

    if(Char.Mem & CWindow::Char::UGlyph)
    {
        Point CXY = Cursor;
        Acc << Char.IconID();
        Write(Acc());
        GotoXY({CXY.X+1,CXY.Y});
    }
    else // Can't have an ASCII character -> and -> a Glyph at the same position.
        (*this) << static_cast<char>(Char.Mem & CWindow::Char::CharMask);
    Cell = Char;
    return *this;
}




Console &Console::operator<<(const char *Text)
{
    Write(Text);
    return *this;
}

void Console::GetGeometry()
{
    if(!Console::Terminal) Console();

#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    Wh = {
            {static_cast<int>(csbi.srWindow.Right - csbi.srWindow.Left + 1), static_cast<int>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)},
            static_cast<int>(csbi.srWindow.Right - csbi.srWindow.Left + 1), static_cast<int>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
    };

#elif defined(__linux__)
    struct winsize win{};
    ioctl(fileno(stdout), TIOCGWINSZ, &win);
    Console::Terminal->Wh = {static_cast<int>(win.ws_col), static_cast<int>(win.ws_row)};
#endif // Windows/Linux

    Book::Debug() << " ScreenSize: " << Color::Yellow << (std::string)Console::Terminal->Wh;
}

Console &Console::operator<<(const char &C)
{
    ::write(1, &C,1);
    Cursor += {1,0};
    return *this;
}

Console::Console()
{
    if(Console::Terminal)
        throw AppBook::Exception() [ Book::Except() << " Multiple instantiation singleton are Fatally rejected!"];
    if(!Console::Terminal) Console::Terminal = this;
}


} // Book::ConIO
#pragma clang diagnostic pop