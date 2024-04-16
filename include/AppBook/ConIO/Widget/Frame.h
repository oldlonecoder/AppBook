//
// Created by oldlonecoder on 24-04-15.
//

//#ifndef APPBOOK_UIFRAME_H
//#define APPBOOK_UIFRAME_H
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
#include <AppBook/ConIO/Widget/Label.h>
#include <AppBook/Utf/Cadres.h>


namespace Book::Ui
{


/*!
 * @brief Framed Rectangular element that can contains several kind of children elements.
 *
 * @author  &copy; 2024, Serge Lussier ( oldlonecoder@oldlonecoder.club )
 *
 */
struct APPBOOK_API Frame : public Element
{

private:
    Label* CaptionLabel{nullptr};

public:
    Frame();
    Frame(Object *parentObj, const std::string &uid, WClass::Type cc);
    Frame(Element *parentObj, const std::string &uid, WClass::Type cc);
    ~Frame() override;

    Utf::Cadre::FrameMat Model{2,2,2,2,0};
    virtual void Show() override;
    virtual Result Render(Rect SubR) override;


    void SetCaption(const std::string& CaptionText);

    void SetIcon(Utf::Glyph::Type Ic);
};

} // Book::Ui

//#endif //APPBOOK_UIFRAME_H
