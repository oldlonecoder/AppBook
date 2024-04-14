//
// Created by oldlonecoder on 24-04-12.
//

//#ifndef APPBOOK_UIELEMENT_H
//#define APPBOOK_UIELEMENT_H

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


struct APPBOOK_EXPORTS UiElement : public Util::Object
{
    Char::Bloc Bloc{nullptr};
    Rect R{};
    Point ScreenXY{1,1}; ///< TopLeft corner of the console screen.
    Char::Type    DefAttr{0x00000020};
    Char::Type    Attr{0x00000000};


    Char::Ptr     EndBloc{nullptr};
    WClass::Type Class{Ui::WClass::Element};

    UiElement() = default;
    ~UiElement() override;

    explicit UiElement(Util::Object* ParentObj, const std::string& UID,      Ui::WClass::Type CC = Ui::WClass::Element);
    UiElement(Book::Ui::UiElement* ParentObj, const std::string& UID, Ui::WClass::Type CC = Ui::WClass::Element);

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


#pragma endregion Drawings


protected:
    Book::Result Alloc();

    Book::Result Resize();
    static Util::Object::Array Gc;
    static size_t GcPush(UiElement* E);

};


struct Console
{
    static Point Cursor;
    static Dim Wh;

    static Book::Result  GetGeometry();
    static Book::Result GotoXY(const Point &XY);
    static void Home();

    static Book::Result RenderElement(UiElement* El, Rect /*SubR*/);
    static void SetBackgroundColor(Color::Code Color);
    static void SetForegroundColor(Color::Code Color);
    static size_t Write(const  std::string& Text, bool isGlyph=false);
    static void DrawFrame(UiElement* El);
    static void UseColors(Char* E);
    static void SetUnderline(bool U);

};



} // Book::ConIO

//#endif //APPBOOK_UIELEMENT_H
