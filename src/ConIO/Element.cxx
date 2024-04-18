//
// Created by oldlonecoder on 24-04-12.
//

#include "AppBook/ConIO/Element.h"
#include <AppBook/Utf/Cadres.h>
#include <AppBook/ConIO/Widget/Icon.h>

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



Util::Object::Array Console::Gc{};


Element::~Element()
{
    delete [] Bloc;
    Bloc = nullptr;
    StrAcc Msg;
    Msg << Color::Red << " ~" << Color::Grey100 << Id() << Color::MediumPurple1 << " Disposing Bloc -> " << R.to_string();
    //Book::Debug()  << Color::Red << " ~" << Color::Grey100 << Id() << Color::MediumPurple1 << " Disposing Bloc -> " << Book::Result::Done;
    Book::Debug()  << Msg << Book::Result::Done;
}

Element::Element(Util::Object *ParentObj, const std::string &UID, Ui::WClass::Type CC): Util::Object(ParentObj, UID),Class(CC | Ui::WClass::Element)
{
    Attr = Char(Attr).SetBgFg(AttrDB::DB["Element"][State::Active]).M;
}




Element::Element(Book::Ui::Element *ParentObj, const std::string &UID, Ui::WClass::Type CC):Util::Object(ParentObj, UID),Class(CC | Ui::WClass::Element){}

void Element::SetGeometry(const Dim& Geo)
{
    R = {Point{0,0},Geo};
    Resize();
}


size_t Element::Alloc()
{
    //if(Bloc)
    delete [] Bloc;
    Bloc = new Char[R.Dwh.Area()+R.Width()];
    Char::Ptr C = Bloc;
    EndBloc = Bloc + R.Dwh.Area();
    for(;C < EndBloc; C++) C->M = Attr;
    return R.Dwh.Area()+R.Width();
}

size_t Element::Resize()
{
    return Alloc();
}

size_t Console::GcPush(Element* E)
{
    for(auto* El : Console::Gc) if(E == El) return 0;
    //E->Detach();
    Console::Gc.push_back(E);
    return Console::Gc.size();
}

size_t Element::PurgeGc()
{
    auto Sz = Console::Gc.size();
    for(auto* El: Console::Gc) delete El;
    return Sz;
}

Book::Result Element::Dispose()
{
    Console::GcPush(this);
    for(auto Child : ChildrenOfType<Element>()) Child->Dispose();
    return Result::Ok;
}

void Element::SetPosition(Point XY)
{
    PosXY = XY;
}


Book::Result Element::GotoXY(const Point &XY)
{
    if(!R.GotoXY(XY)) return Book::Result::Rejected;
    return Result::Accepted;
}

void Element::PutGlyph(Utf::Glyph::Type G)
{
    if(Char::Ptr P = At(); P) P->SetGlyph(G) | Attr;
}

Book::Result Element::WriteStr(const std::string &Txt)
{
    Char::Ptr P = At();
    for(auto C: Txt)
    {
        if(P > EndBloc) return Book::Result::Overflow;
        P->M |= (P->M & ~(Char::UtfMask | Char::BgFgMask)) | Attr | C;
        ++P;
    }
    return Result::Ok;
}

Char::Ptr Element::At(const Point &XY) const
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

void Element::SetFgColor(Color::Code C)
{
    if(!R[R.Cursor]) return;
    //At()->SetFg(C);
    Attr = Char(Attr).SetFg(C).M;
}


void Element::SetBgColor(Color::Code C)
{
    if(!R[R.Cursor]) return;
    //At()->SetBg(C);
    Attr = Char(Attr).SetBg(C).M;
}

void Element::SetColors(Color::Pair Cp)
{
    SetBgColor(Cp.Bg);
    SetFgColor(Cp.Fg);
}

void Element::Clear() const
{
    for(Char::Ptr P = Bloc; P <= EndBloc; P++) P->M = Attr | 0x20;
}




// Mostly for Frame Drawing/ Drawing on and around the Frame.
void Element::TopLeft()
{
    R.GotoXY(R.A);
}

void Element::TopRight()
{
    R.GotoXY({R.B.X, R.A.Y});
}

void Element::Top()
{
    R.GotoXY({R.Cursor.X,R.A.Y});
}

void Element::Bottom()
{
    R.GotoXY({R.Cursor.X,R.B.Y});
}

void Element::BottomLeft()
{
    R.GotoXY({R.A.X,R.B.Y});
}

void Element::BottomRight()
{
    R.GotoXY(R.B);
}

void Element::Show()
{
    //...
    Clear();
    for(auto Child: ChildrenOfType<Element>()) Child->Show();
    //...
}

Book::Result Element::Render(Rect SubR)
{

    Console::RenderElement(this);
    auto ChildrenEl = ChildrenOfType<Element>();

    for(auto Child : ChildrenEl)
    {
        if(Child->Class & WClass::Element)
            Child->Render({});
    }
    return Result::Done;
}

Point Element::GetScreenXY()
{
    Point XY = PosXY;
    auto ParentUI = Parent<Element>();
    while(ParentUI)
    {
        XY += ParentUI->PosXY;
        ParentUI = ParentUI->Parent<Element>();
    }
    return XY;
}

Element::Array Element::QueryTypes(WClass::Type Types)
{
    Array List{};
    for(auto El: ChildrenOfType<Element>())
        if(El->Class == Types) List.emplace_back(El); // Strict comparison !
    if(List.empty())
        Message() << " No such children of type " << static_cast<int>(Types);
    //...
    return List;
}

int Element::Width()
{
    return R.Width(); // For now...
}

int Element::Height()
{
    return R.Height(); //...
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
 * @note This version does not invalidate nor execute Rectangle:Intersection operations to adjust those dimensions because
 *       it is completely two different things. This Book::Ui version is NOT a CUI Window/Ui Manager.
 *
 */
Book::Result Console::RenderElement(Element *El)
{
    auto ScreenXY = El->GetScreenXY();
    GotoXY(ScreenXY);
    El->R.Home();
    auto PrevCell = &El->Bloc[0];
    Console::UseColors(PrevCell);

    for(int Line=0; Line< El->R.Height(); Line++)
    {
        El->R.GotoXY({El->R.A.X,El->R.A.Y+Line});
        Console::GotoXY(Point(El->R.A.X, El->R.A.Y+Line) + ScreenXY);
        Char* Cell = El->At();

        Console::SetColors(El->Attr);
        for(int Col=0; Col<El->R.Width(); Col++)
        {
            if(Cell->BackgroundColor() != PrevCell->BackgroundColor())
                Console::SetBackgroundColor(Cell->BackgroundColor());
            if(Cell->ForegroundColor() != PrevCell->ForegroundColor())
                Console::SetForegroundColor(Cell->ForegroundColor());
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
            if(Cell->M & Char::Underline)
            {
                Console::SetUnderline(true);
            }
            /// No Cadre components here...
            char C = static_cast<char>(Cell->M&0xFF);
            write(1,&C,1);
            Console::Cursor.X++;
            PrevCell = Cell++; //Oooooopsyyyy!
        }
        write(1,"\x1b[0m", 4);

    }
//    if(El->Class & Ui::WClass::Frame)
//        Console::DrawFrame(El);
    //else
    if(El->Class & WClass::Glyph)
    {
        Console::SetColors(El->Attr);
        Console::GotoXY(ScreenXY);
        Write(Utf::Glyph::Data[El->As<Ui::Icon>()->Ic],true);
    }
    // Terminate and reset colors and other attributes to not explode them in between the next modifications.
    std::cout << "\x1b[0m";
    return Result::Done;
}


Book::Result Console::RenderElement(Element *El, Rect SubRect)
{
    // Setup Local and Screen console geometry :
    // Start with the Element itself:
    auto ScreenRect = Rect(SubRect + El->GetScreenXY());
    // Offset SubRect from ScreenRect:
    auto ScreenSubRect = ScreenRect + SubRect;
    // Then Clip ( get intersection ) with the ScreenRect
    auto Screen = ScreenRect / ScreenSubRect;
    if(!Screen) return Result::Rejected; // SubRegion and/or Element geometry are not "visible".
    // Re-adjust Local geometry:

    //...Then before trying to output the stuff, let's see the values:


    return Result::Done;
}



void Console::SetBackgroundColor(Color::Code Color)
{
    auto Acc = Color::AnsiBg(Color);
    write(1, Acc.c_str(), Acc.length());
}

void Console::SetForegroundColor(Color::Code Color)
{
    auto Acc = Color::Ansi(Color);
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
    auto sz = Text.length();
    write(1, Text.c_str(), sz);
    if(isGlyph)
    {
        Cursor.X++;
        return sz;
    }
    Cursor.X += (int)sz;
    return 0;
}

void Console::DrawFrame(Element *El)
{
    Utf::Cadre Cadre;
    auto ScreenXY = El->GetScreenXY();
    //Cadre = {2,2,2,2,0};
    Console::GotoXY(ScreenXY);
    auto Cp = AttrDB::DB["Frame"][State::Active];

    Console::SetColors(Cp);
    //Console::SetBackgroundColor(Color::Blue);
    //Console::SetForegroundColor(Color::Grey100);
    Console::Write(Cadre[Utf::Cadre::TopLeft]);

    El->TopRight();
    Console::GotoXY(ScreenXY+El->R.Cursor);
    Console::Write(Cadre[Utf::Cadre::TopRight]);

    El->BottomLeft();
    Console::GotoXY(ScreenXY+El->R.Cursor);
    Console::Write(Cadre[Utf::Cadre::BottomLeft]);

    El->BottomRight();
    Console::GotoXY(ScreenXY+El->R.Cursor);
    Console::Write(Cadre[Utf::Cadre::BottomRight]);

    for(int Col=1; Col<El->R.Width()-1; Col++)
    {
        Console::GotoXY(ScreenXY + Point{Col, 0});
        Console::Write(Cadre[Utf::Cadre::Horizontal]);
        Console::GotoXY(ScreenXY + Point{Col, El->R.B.Y});
        Console::Write(Cadre[Utf::Cadre::Horizontal]);
    }

    for(int Line=1; Line < El->R.Height()-1; Line++)
    {
        Console::GotoXY(ScreenXY + Point{El->R.A.X, El->R.A.Y+Line});
        Console::Write(Cadre[Utf::Cadre::Vertical]);
        Console::GotoXY(ScreenXY + Point{El->R.B.X, El->R.A.Y+Line});
        Console::Write(Cadre[Utf::Cadre::Vertical]);

    }

}

void Console::UseColors(Char *E)
{
    Console::SetBackgroundColor(E->BackgroundColor());
    Console::SetForegroundColor(E->ForegroundColor());
}

void Console::SetUnderline(bool U)
{
    if(U)
        write(1, "\x1b[4m", 4);
    else
        write(1, "\x1b[0m", 4);
}

void Console::SetColors(Char::Type E)
{
    auto Cp = Char(E).Colors(E);
    Console::SetColors(Cp);

}

void Console::SetColors(Color::Pair Cp)
{
    auto Text = Cp();
    write(1,Text.c_str(), Text.length());

}



} // Book::ConIO

