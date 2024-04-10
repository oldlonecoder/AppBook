//
// Created by oldlonecoder on 24-04-10.
//

#include "AppBook/Console/Console.h"

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




#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
Book::Result Console::GotoXY(const Point &XY) // NOLINT(*-make-member-function-const)
{
//    if(!Rect({0,0},Wh)[XY])
//    {
//        Book::Error() << Result::Rejected << " coord '" << (XY + Point(1,1)) << "' is out of console's geometry.";
//        return Book::Result::Rejected;
//    }
    Cursor = XY;
    StrAcc Acc = "\x1b[%d;%dH";
    Acc << XY.Y+1 << XY.X + 1;
    std::cout << Acc();
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
    Cursor = W->R.A;
    GotoXY(Cursor);
    SetColor(W->Buffer[0][0].Colors());

    Book::Debug() << " CWindow Coordinates:" << W->R.A;

    for(auto &Line : W->Buffer)
    {
        GotoXY(Cursor);
        for(auto Col: Line) Write(Col);
        write(1,"\033[0m\n",5);
        Cursor = {W->R.A.X,Cursor.Y+1};
        //Cursor.X=0;
    }
    fflush(stdout);
}



void Console::GetGeometry()
{


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
    Wh = {static_cast<int>(win.ws_col), static_cast<int>(win.ws_row)};
#endif // Windows/Linux

    Book::Debug() << " ScreenSize: " << Color::Yellow << Wh;
}


size_t Console::Write(const char *Text)
{
    return Write(std::string(Text));
}

size_t Console::Write(const char &Char8)
{
    auto sz = ::write(1,&Char8,1);
    Cursor += {1,0};
    return sz;
};

void Console::SetFgColor(Color::Code Code)
{
    auto Seq = Color::Ansi(Code);
    write(1,Seq.c_str(), Seq.length());
    fflush(stdout);
}

void Console::SetColor(Color::Pair BgFg)
{
    auto Seq = BgFg();
    write(1,Seq.c_str(), Seq.length());
    fflush(stdout);
}

size_t Console::Write(CWindow::Char Char)
{
    static CWindow::Char Cell;
    StrAcc Acc{};
    size_t sz{0};
    if(Char.Bg() != Cell.Bg())
    {
        SetBgColor(Char.Bg());
        Cell = Char;
    }
    if(Char.Fg() != Cell.Fg()) {
        SetFgColor(Char.Fg());// Color::Ansi(D.Fg());
        Cell = Char;
    }
    if(Char.Mem & CWindow::Char::Frame)
        return Write(Utf::Cadre()[static_cast<Utf::Cadre::Index>(Char.Mem & CWindow::Char::CharMask)]);

    if(Char.Mem & CWindow::Char::UGlyph)
        return DrawIcon(Char.IconID());

    return Write(static_cast<char>(Char.Mem & CWindow::Char::CharMask));
}

void Console::SetBgColor(Color::Code Code)
{
    auto Seq = Color::AnsiBg(Code);
    write(1,Seq.c_str(), Seq.length());
    fflush(stdout);
}

size_t Console::DrawIcon(Utf::Glyph::Type IcID)
{
    auto Seq = std::string(Utf::Glyph::Data[IcID]);
    write(1,Seq.c_str(), Seq.length());
    //Cursor += {1,0};
    GotoXY(Cursor);
    return Seq.length();
}


} // Book::ConIO
#pragma clang diagnostic pop