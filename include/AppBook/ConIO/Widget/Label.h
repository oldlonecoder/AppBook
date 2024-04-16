//
// Created by oldlonecoder on 24-04-14.
//

//#ifndef APPBOOK_LABEL_H
//#define APPBOOK_LABEL_H
/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once
#include <AppBook/ConIO/Widget/Icon.h>


namespace Book::Ui
{

struct APPBOOK_API Label: Element
{

    Icon* LeftIcon{nullptr};
    Icon* RightIcon{nullptr};

    StrAcc Text{};
    Label(Object* ParentObj, const std::string& LblText);
    ~Label() override;

    void Show() override;
    void SetLeftGlyph(Utf::Glyph::Type G, Color::Pair C);
    void SetText(const std::string& NewText);

    virtual Result Render(Rect SubR) override;
};

} // Book::Ui

//#endif //APPBOOK_LABEL_H
