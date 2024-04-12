//
// Created by oldlonecoder on 24-04-12.
//

#include "AppBook/ConsoleUI/UiElement.h"

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
    if(Bloc) delete [] Bloc;
    Bloc = new Char::Type[R.Dwh.Area()+R.Width()];
    Char::Ptr C = Bloc;
    EndBloc = Bloc + R.Dwh.Area();
    for(;C < EndBloc; C++) *C = DefAttr;
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
    if(Char::Ptr P = At(); P) *P = (Char(*P).SetGlyph(G)).M;
}

Book::Result UiElement::WriteStr(const std::string &Txt)
{
    Char::Ptr P = At();
    for(auto C: Txt)
    {
        if(P > EndBloc) return Book::Result::Overflow;
        *P++ = (Char(*P) | (Char::Type)C).M;
    }
    return Result::Ok;
}

Char::Ptr UiElement::At(const Point &XY)
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
    Char::Ptr  P{At()};
    *P = (Char(*P).SetFg(C)).M;
}


void UiElement::SetBgColor(Color::Code C)
{
    if(!R[R.Cursor]) return;
    Char::Ptr  P{At()};
    *P = (Char(*P).SetBg(C)).M;
}

void UiElement::SetColors(Color::Pair Cp)
{
    SetBgColor(Cp.Bg);
    SetFgColor(Cp.Fg);
}

void UiElement::Clear()
{
    for(Char::Ptr P = Bloc; P <= EndBloc; P++)
    {
        *P = DefAttr;
    }
}


} // Book::ConsoleUI

