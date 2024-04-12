//
// Created by oldlonecoder on 24-04-08.
//

//#ifndef APPBOOK_CWINDOW_H
//#define APPBOOK_CWINDOW_H

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
#include <AppBook/Util/Geometry.h>
#include <AppBook/Util/Object.h>
#include <AppBook/Utf/Cadres.h>
#include <AppBook/Utf/Glyphes.h>
#include <AppBook/Console/UiEnums.h>


namespace Book::ConIO
{


class APPBOOK_EXPORTS Window: public Util::Object
{
    Point        ScreenXY{};
    Rect                R{};  ///< Also contains inner Cursor
    Ui::WClass::Type    Flags{ Ui::WClass::Frame };
    Color::Pair         Colors{Color::Reset,Color::Reset};

    friend struct Console;
    static Rect ScreenRect;
public:


    Window() = default;
    Window(Util::Object* ParentObj, const std::string& Id);
    ~Window() override;

    void SetScreenPosition(const Point& XY) { ScreenXY = XY; }
    void Clear();
    void SetGeometry(Point Geo);
    int Width();
    int Height();

    struct APPBOOK_EXPORTS Pencil
    {
        Window* Win{nullptr};
        Rect     R{};
        [[nodiscard]] int Width() const;
        [[nodiscard]] int Height() const;

        Pencil(Window* W, const Color::Pair& DefaultAttr, Rect Sub);
        ~Pencil() = default;

        Pencil& operator << (const std::string& Input);
        Pencil& operator << (char C);
        Pencil& operator << (Color::Code C);
        Pencil& operator << (Color::Pair Colors);
        Pencil& operator << (Utf::Glyph::Type Ic);
        Pencil& operator << (Utf::AccentFR::Type Ac);
        Pencil& operator << (Utf::Cadre::Index If);

        Book::Result Position(Point XY={});
        void ClearBg(Color::Code C);
        void Clear(Color::Code C);


        bool operator++();
        bool operator++(int);
        bool operator--();
        bool operator--(int);

    };

    Window::Pencil  GetPencil(Rect Geom={}, Color::Pair Attr={});
    Book::Result operator >>(StrAcc& Acc);
    void DrawFrame();
    void Draw(const Rect& SubR={});


private:





};

} // Book::ConIO


//#endif //APPBOOK_CWINDOW_H
