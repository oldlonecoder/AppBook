//
// Created by oldlonecoder on 24-04-18.
//

//#ifndef APPBOOK_INPUTFIELD_H
//#define APPBOOK_INPUTFIELD_H
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
#include <AppBook/Ui/Widget/Label.h>
#include <AppBook/Utf/Cadres.h>
namespace Book::Ui
{

struct APPBOOK_API InputField : Element
{
    Icon* LeftIcon{nullptr};
    std::string PlaceHolder{};
    Dim InputLengths{};

    enum class Type: uint8_t
    {
        Passwd,
        Email,
        Username,
        Birth,
        Generic,
        //...etc...
    }InType{InputField::Type::Generic};

    enum class Mode : uint8_t
    {
        Echo,
        Random,
        Star
    }InMode{Mode::Echo};

    Utf::Cadre::FrameMat Model={2,0,2,0,0};


    InputField() = default;
    ~InputField() override;

    InputField(Element* ParentEl, const std::string& Uid);
    void SetGeometry(Dim Geo) override;
    Book::Result Render(Rect SubR) override;




    //...
};


} // Book::Ui

//#endif //APPBOOK_INPUTFIELD_H
