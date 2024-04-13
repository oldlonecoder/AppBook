//
// Created by oldlonecoder on 24-04-12.
//

#include "AppBook/ConsoleUI/UiElement.h"
#include <AppBook/Utf/Cadres.h>

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




namespace Book::ConsoleUI
{



Util::Object::Array UiElement::Gc{};


UiElement::~UiElement()
{
    delete [] Bloc;
    Bloc = nullptr;
    Book::Debug()  << Color::Red << " ~" << Color::Grey100 << Id() << Color::Yellow << " Disposing Bloc -> " << Book::Result::Finish;
}

UiElement::UiElement(Util::Object *ParentObj, const std::string &UID, Ui::WClass::Type CC): Util::Object(ParentObj, UID),Class(CC|Ui::WClass::Element){}




UiElement::UiElement(Book::ConsoleUI::UiElement *ParentObj, const std::string &UID, Ui::WClass::Type CC):Util::Object(ParentObj, UID),Class(CC|Ui::WClass::Element){}

void UiElement::SetGeometry(const Dim& Geo)
{
    R = {Point{0,0},Geo};
    Resize();
}


Book::Result UiElement::Alloc()
{
    //if(Bloc)
    delete [] Bloc;
    Bloc = new Char[R.Dwh.Area()+R.Width()];
    Char::Ptr C = Bloc;
    EndBloc = Bloc + R.Dwh.Area();
    for(;C < EndBloc; C++) C->M = DefAttr;
    return Result::Ok;
}

Book::Result UiElement::Resize()
{
    return Alloc();
}

size_t UiElement::GcPush(UiElement* E)
{
    for(auto* El : UiElement::Gc) if(E == El) return 0;
    E->Detach();
    UiElement::Gc.push_back(E);
    return UiElement::Gc.size();
}

size_t UiElement::PurgeGc()
{
    auto Sz = UiElement::Gc.size();
    for(auto* El: UiElement::Gc) delete El;
    return Sz;
}

Book::Result UiElement::Dispose()
{
    UiElement::GcPush(this);
    return Result::Ok;
}



Book::Result UiElement::GotoXY(const Point &XY)
{
    if(!R.GotoXY(XY)) return Book::Result::Rejected;
    return Result::Accepted;
}

void UiElement::PutGlyph(Utf::Glyph::Type G)
{
    if(Char::Ptr P = At(); P) P->SetGlyph(G) | Attr;
}

Book::Result UiElement::WriteStr(const std::string &Txt)
{
    Char::Ptr P = At();
    for(auto C: Txt)
    {
        if(P > EndBloc) return Book::Result::Overflow;
        P->M |= (P->M& ~(Char::UtfMask | Char::BgFgMask)) | Attr | C;
        ++P;
    }
    return Result::Ok;
}

Char::Ptr UiElement::At(const Point &XY) const
{
    Point CXY;
    if(!R[XY])
        CXY = R.Cursor;
    else
        CXY = XY;

    if(!R[R.Cursor])
    {
        Book::Error() << Book::Result::Overflow << " @" << CXY;
        return nullptr;
    }
    return Bloc + R.Width() * CXY.Y + CXY.X;
}

void UiElement::SetFgColor(Color::Code C)
{
    if(!R[R.Cursor]) return;
    At()->SetFg(C);
    Attr = Char(Attr).SetFg(C).M;
}


void UiElement::SetBgColor(Color::Code C)
{
    if(!R[R.Cursor]) return;
    At()->SetFg(C);
    Attr = Char(Attr).SetFg(C).M;
}

void UiElement::SetColors(Color::Pair Cp)
{
    SetBgColor(Cp.Bg);
    SetFgColor(Cp.Fg);
}

void UiElement::Clear() const
{
    for(Char::Ptr P = Bloc; P <= EndBloc; P++) P->M = Attr | 0x20;
}




// Mostly for Frame Drawing/ Drawing on and around the Frame.
void UiElement::TopLeft()
{
    R.GotoXY(R.A);
}

void UiElement::TopRight()
{
    R.GotoXY({R.B.X, R.A.Y});
}

void UiElement::Top()
{
    R.GotoXY({R.Cursor.X,R.A.Y});
}

void UiElement::Bottom()
{
    R.GotoXY({R.Cursor.X,R.B.Y});
}

void UiElement::BottomLeft()
{
    R.GotoXY({R.A.X,R.B.Y});
}

void UiElement::BottomRight()
{
    R.GotoXY(R.B);
}


// -------------------------------------------------------- Console ----------------------------------------------------------


Point Console::Cursor{1,1};
Dim   Console::Wh{0,0};






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



Book::Result Console::GetGeometry()
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
    Console::Wh = {static_cast<int>(win.ws_col), static_cast<int>(win.ws_row)};
#endif // Windows/Linux

    Book::Debug() << " ScreenSize: " << Color::Yellow << Console::Wh;
    return Result::Done;
}


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

void Console::Home()
{
    GotoXY({1,1});
}


/*!
 * @brief Render Ui Element on console screen
 * @param El
 * @return Always return Book::Result::Done;
 * @note Attention: Actually developing and experimenting this Console code... There is nothing about Rectangle managements ( Clipping/intersection/visibility...etc...) !!
 */
Book::Result Console::RenderElement(UiElement *El, Rect)
{
    GotoXY(El->ScreenXY /* + SubR.A */);
    /* Rect ScreenArea = Rect({0,0},Wh) / SubR; */
    El->R.Home();
    auto PrevCell = El->At();
    Console::SetBackgroundColor(El->Bloc[0].BackgroundColor());
    Console::SetForegroundColor(El->Bloc[0].ForegroundColor());

    for(int Line=0; Line< El->R.Height(); Line++)
    {
        El->R.GotoXY({El->R.A.X,El->R.A.Y+Line});
        Console::GotoXY(Point(Point(El->R.A.X, El->R.A.Y+Line) + El->ScreenXY));
        Char* Cell = El->At();
        PrevCell->SetBgFg({Color::Reset,Color::Reset});
        for(int Col=0; Col<El->R.Width(); Col++)
        {


            if(Cell->BackgroundColor() != PrevCell->BackgroundColor())
                SetBackgroundColor(Cell->BackgroundColor());
            if(Cell->ForegroundColor() != PrevCell->ForegroundColor())
                SetForegroundColor(Cell->ForegroundColor());
            if(auto [b,AccStr] = Cell->AccentFlag(); b)
            {
                Write(AccStr);
                ++Cell;
                continue;
            }
            if(auto [b,IcStr] = Cell->Graphen(); b)
            {
                Write(IcStr,true);
                ++Cell;
                continue;
            }
            /// No Cadre components here...
            write(1,(char*)&Cell->M,1);
            Console::Cursor.X++;
            Cell++; //Oooooopsyyyy!
        }
        write(1,"\x1b[0m", 4);

    }
    return Result::Done;
}

void Console::SetBackgroundColor(Color::Code Color)
{
    auto Acc = Color::Ansi(Color);
    write(1, Acc.c_str(), Acc.length());
}

void Console::SetForegroundColor(Color::Code Color)
{
    auto Acc = Color::AnsiBg(Color);
    write(1, Acc.c_str(), Acc.length());
}


/*!
 * @brief Commit contents on screen
 * @param Text
 * @param isGlyph
 * @return size of text.
 * @note As of this stage, nothing is checked.
 */
size_t Console::Write(const std::string &Text, bool isGlyph)
{
    auto sz = write(1, Text.c_str(), Text.length());
    if(isGlyph)
    {
        Cursor.X++;
        return sz;
    }
    Cursor.X += (int)sz;
    return 0;
}


} // Book::ConsoleUI

