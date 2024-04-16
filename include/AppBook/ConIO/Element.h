//
// Created by oldlonecoder on 24-04-12.
//

//#ifndef APPBOOK_ELEMENT_H
//#define APPBOOK_ELEMENT_H

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

#include <AppBook/Util/Object.h>
#include <AppBook/ConIO/Char.h>




namespace Book::Ui
{


struct APPBOOK_API Element : public Util::Object
{
    Char::Bloc Bloc{nullptr};
    Rect R{};
    Point PosXY{0,0}; ///< TopLeft corner of the console screen.


    Char::Type    Attr{0x00000020};


    Char::Ptr     EndBloc{nullptr};
    WClass::Type Class{Ui::WClass::Element};
    State::Type  St{State::Normal};

    Element() = default;
    ~Element() override;

    explicit Element(Util::Object* ParentObj, const std::string& UID, Ui::WClass::Type CC = Ui::WClass::Element);
    Element(Book::Ui::Element* ParentObj, const std::string& UID, Ui::WClass::Type CC = Ui::WClass::Element);

    void SetGeometry(const Dim& Geo);
    Book::Result Dispose();
    static size_t PurgeGc();
    Char::Ptr At(const Point& XY = {} ) const;
#pragma region Drawings
    Book::Result GotoXY(const Point& XY);
    void SetFgColor(Color::Code C);
    void SetBgColor(Color::Code C);
    void SetColors(Color::Pair Cp);

    void PutGlyph(Utf::Glyph::Type G);
    Book::Result WriteStr(const std::string& Txt);
    void Clear() const;
    void TopLeft();
    void TopRight();
    void Top();
    void Bottom();
    void BottomLeft();
    void BottomRight();
    virtual void Show();
    Point GetScreenXY();
    int Width();
    int Height();
    Element::Array QueryTypes(WClass::Type Types);
#pragma endregion Drawings

    void SetPosition(Point XY);
    virtual Book::Result Render(Rect SubR);
protected:

    size_t Alloc();
    size_t Resize();
};


struct Console
{
    static Point Cursor;
    static Dim Wh;

    static Book::Result  GetGeometry();
    static Book::Result GotoXY(const Point &XY);
    static void Home();

    static Book::Result RenderElement(Element* El, Rect /*SubR*/);
    static void SetBackgroundColor(Color::Code Color);
    static void SetForegroundColor(Color::Code Color);
    static size_t Write(const  std::string& Text, bool isGlyph=false);
    static void DrawFrame(Element* El);
    static void UseColors(Char* E);
    static void SetColors(Char::Type E);
    static void SetColors(Color::Pair Cp);
    static void SetUnderline(bool U);
    static Util::Object::Array Gc;
    static size_t GcPush(Element* E);
};



} // Book::ConIO

//#endif //APPBOOK_ELEMENT_H
