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

void Console::Render(Window *W, Rect /*SubR*/)
{
    Cursor = W->ScreenXY;
    W->R.Home();
    //GotoXY(Cursor);
    SetColor(Window::Char(W->Buffer[0]).Colors());
    Window::Char PCell{*W->Peek(W->R)};
    Book::Debug() << " Window Coordinates:" << W->ScreenXY;

    for(int Line = 0; Line < W->R.Height(); Line++)
    {
        GotoXY(Cursor);
        Window::Type P = W->Peek(W->R);
        for(int Col=0; Col< W->Width(); Col++)
        {
            Window::Char Char(*P++);
            StrAcc Acc{};
            if(Char.Bg() != PCell.Bg())
            {
                SetBgColor(Char.Bg());
                PCell = Char;
            }
            if(Char.Fg() != PCell.Fg()) {
                SetFgColor(Char.Fg());// Color::Ansi(D.Fg());
                PCell = Char;
            }
            if(*Char & Window::Char::Frame) {
                auto Str = Utf::Cadre()[static_cast<Utf::Cadre::Index>(Char.Mem & Window::Char::CharMask)];
                write(1, Str.c_str(), Str.length());
                Cursor += {1,0};
                ++W->R;
                continue;
            }
            if(*Char & Window::Char::UGlyph) {
                DrawIcon(Char.IconID());
                ++W->R;
                //Cursor += {1,0};
            }
            else
                Write(static_cast<char>(*Char & Window::Char::CharMask));
            ++W->R;
            Cursor += {1,0};
        }
        write(1,"\033[0m\n",5);
        Cursor = {W->ScreenXY.X,Cursor.Y+1};
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

    return sz;
}

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
    GotoXY(Cursor);
    return Seq.length();
}


} // Book::ConIO
#pragma clang diagnostic pop