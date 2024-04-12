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
#include <AppBook/ConsoleUI/Char.h>




namespace Book::ConsoleUI
{


struct APPBOOK_EXPORTS UiElement : public Util::Object
{
    Char::Bloc Bloc{nullptr};
    Rect R{};
    Point ScreenXY{};
    Char::Type    DefAttr{0x00FFFF20};
    Char::Ptr     EndBloc{nullptr};
    Ui::WClass::Type Class{Ui::WClass::Element};

    UiElement() = default;
    ~UiElement() override;

    explicit UiElement(Util::Object* ParentObj, const std::string& UID, Ui::WClass::Type CC = {Ui::WClass::Element});
    UiElement(Book::ConsoleUI::UiElement* ParentObj, const std::string& UID, Ui::WClass::Type CC = {Ui::WClass::Element});

    void SetGeometry(const Dim& Geo);
    Book::Result Dispose();
    static size_t PurgeGc();
    Char::Ptr At(const Point& XY = {} );
#pragma region Drawings
    Book::Result GotoXY(const Point& XY);
    void SetFgColor(Color::Code C);
    void SetBgColor(Color::Code C);
    void SetColors(Color::Pair Cp);

    void PutGlyph(Utf::Glyph::Type G);
    Book::Result WriteStr(const std::string& Txt);
    void Clear();


#pragma endregion Drawings


protected:
    Book::Result Alloc();

    Book::Result Resize();
    static Util::Object::Array Gc;
    static size_t GcPush(UiElement* E);

};

} // Book::ConsoleUI

//#endif //APPBOOK_UIELEMENT_H
