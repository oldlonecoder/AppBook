//
// Created by oldlonecoder on 24-04-08.
//

#include "AppBook/Console/Window.h"
#include <AppBook/Book/ApplicationBase.h>




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



// Boook !@! lol


namespace Book::ConIO
{

Rect Window::ScreenRect{};

#pragma region CWindow

Window::Window(Util::Object *ParentObj, const std::string &Id) : Object(ParentObj, Id){}

Window::~Window()
{
    Detach();
}




void Window::Clear()
{
    auto Area = R.Dwh.Area();
    for(int X=0; X < Area; X++) Buffer[X] = (A & ~Char::CharMask) | 0x20;
}

void Window::SetGeometry(Point Geo)
{
    R = Rect({0,0},Geo);
    ReAlloc();
}


int Window::Width()
{
    return R.Width();
}

int Window::Height()
{
    return R.Height();
}


Book::Result Window::ReAlloc()
{
    if(Buffer) delete [] Buffer;
    return Alloc();
}


Window::Pencil Window::GetPencil(Rect Geom, Window::Char::Type Attr)
{
    if(!Geom)
    {
        Geom = R;
        Geom -= R.A;
    }
    return Pencil(this, Attr,Geom);

}


/*!
 * @brief Render inner Buffer to a StrAcc.
 * @param Acc
 * @return Always return Result::Ok ...
 * @note Attention: Glyphes (Icons other than Accents) are forbidden using string unless the final output ( to the console gui )
 *       is controlled by the Book::Console Renderer.
 */
Book::Result Window::operator>>(StrAcc &Acc)
{
    //int Glyph=0;
    Utf::Cadre Cadre;
    Type PrevCell = Buffer;
    for(int Line = 0; Line < R.Height(); Line++)
    {
        Type Crs = Peek();
        for(int Col=0; Col < R.Width(); Col++)
        {

            if(*Crs & Window::Char::Frame)
            {
                Acc << Cadre[static_cast<Utf::Cadre::Index>(*Crs++ & Window::Char::CharMask)];
                ++R;
                continue; // Colors or any other attributes changes on Frame is bypassed (rejected).
            }
            if(Char(*Crs).Bg() != Char(*PrevCell).Bg())
                Acc << Color::AnsiBg(Char(*Crs).Bg());
            if(Char(*Crs).Fg() != Char(*PrevCell).Fg())
                Acc << Char(*Crs).Fg();
            if(*Crs & Window::Char::UGlyph)
            {
                Acc << Char(*Crs).IconID();

            }
            else // Can't have an ASCII character -> and -> a Glyph at the same position.
                Acc << static_cast<char>(*Crs & Window::Char::CharMask);

            PrevCell = Crs++;
            ++R;
        }
        Acc << Color::Reset << '\n'; ///@note  Ambiguous Assignation because This 2D text would be written also in logging journal, for ex.: in this AppBook API/Framework
    }
    return Result::Ok;
}

void Window::DrawFrame()
{
    if(!R)
        throw AppBook::Exception()[ Book::Except() << Utf::Glyph::Bomb << " Cannot draw a frame with no dimensions!"];

    Utf::Cadre Cdr;
    auto Painter = GetPencil();
    Painter.Position(R.A);
    Painter << Color::Pair({Color::Grey100,Color::Reset});
    Painter << Utf::Cadre::TopLeft;
    Painter.Position({R.B.X, R.A.Y});
    Painter << Utf::Cadre::TopRight;
    Painter.Position({R.A.X, R.B.Y});
    Painter << Utf::Cadre::BottomLeft;
    Painter.Position({R.B.X, R.B.Y});
    Painter << Utf::Cadre::BottomRight;
    for(int X=1; X<R.Width()-1; X++)
    {
        Painter.Position({X,R.A.Y});
        Painter << Utf::Cadre::Horizontal;
        Painter.Position({X,R.B.Y});
        Painter << Utf::Cadre::Horizontal;
    }
    for(int Y=1; Y<R.Height()-1; Y++)
    {
        Painter.Position({R.A.X,Y});
        Painter << Utf::Cadre::Vertical;
        Painter.Position({R.B.X,Y});
        Painter << Utf::Cadre::Vertical;
    }

}

void Window::Draw(const Rect& SubR)
{
    ApplicationBase::Instance().Console().Render(this, SubR);
}

Window::Type Window::Peek(const Rect& Area)
{
    if(Area) {
        if (!R[Area.Cursor])
            throw AppBook::Exception()[Book::Error() << " Peek at given coords are outside of this Window boundaries.(" << Area.Cursor << ") <--> (" << R << ")"];
        return Buffer + Area.Cursor.Y*R.Width() + Area.Cursor.X;
    }
    return Buffer + R.Cursor.Y*R.Width() + R.Cursor.X;
}


#pragma endregion CWindow





} // Book::ConIO

