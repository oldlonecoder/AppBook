//
// Created by oldlonecoder on 24-04-13.
//

//#ifndef APPBOOK_ICON_H
//#define APPBOOK_ICON_H
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

#include <AppBook/ConIO/UiElement.h>



namespace Book::Ui
{

struct APPBOOK_EXPORTS Icon : UiElement
{
    Utf::Glyph::Type Ic{Utf::Glyph::DeadHead};

    Icon() = default;
    Icon(Util::Object* ParentObj);
    ~Icon() override = default;

    Icon& operator = (Utf::Glyph::Type IIc);
    //...
    std::string operator ()();


};

} // Book::Ui


//#endif //APPBOOK_ICON_H
