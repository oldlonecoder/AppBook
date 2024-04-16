//
// Created by oldlonecoder on 24-04-15.
//

#include "AppBook/ConIO/Widget/Frame.h"

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




namespace Book::Ui
{


Frame::Frame()
{}




Frame::Frame(Util::Object *parentObj, const std::string &uid, WClass::Type cc) : Element(parentObj, uid, cc | WClass::Frame)
{
    Attr = Char(Attr).SetBgFg(AttrDB::DB["Frame"][State::Active]).M;
}

Frame::Frame(Element *parentObj, const std::string &uid, WClass::Type cc) : Element(parentObj, uid, cc | WClass::Frame)
{
    Attr = Char(Attr).SetBgFg(AttrDB::DB["Frame"][State::Active]).M;
}

Frame::~Frame()
{
}

void Frame::Show()
{
    Element::Show();

}

Result Frame::Render(Rect)
{
    Element::Render({});
    Console::DrawFrame(this);
    //... Then our children elements that draws on our frame.
    auto List = QueryTypes(WClass::FrameComponent|WClass::Caption/*...more to come*/);
    if(!List.empty())
    {
        for (auto O: List)
        {
            auto FrComp = O->As<Element>();
            if(FrComp->Class & WClass::Caption)
            {
                Utf::Cadre Cadre;
                Model = Cadre.Model;
                Cadre.Model = {2,1,0,1,0};
                Cadre = Cadre.Model;
                Point ScreenXY = FrComp->GetScreenXY();
                Console::GotoXY(ScreenXY - Point{1,0});
                Console::SetColors(AttrDB::DB["Frame"][State::Active]); // Arbitrary "State" because ...There is nos States yet! lol
                Console::Write(Cadre[Utf::Cadre::VerticalTowardsLeft]);
                ScreenXY += {FrComp->Width()+1,0};
                Console::GotoXY(ScreenXY- Point{1,0});
                Console::Write(Cadre[Utf::Cadre::VerticalTowardsRight]);
                Cadre = Model;
            }
                FrComp->Render({});

        }
    }
    return Result::Done;
}

void Frame::SetCaption(const std::string &CaptionText)
{
    if(CaptionText.empty()){;}
    auto List = QueryTypes(WClass::Caption|WClass::FrameComponent);
    if(!List.empty())
    {
        auto CapObj = List[0]->As<Label>(); // Ignore all other  Captions - in fact there is one and only one caption on a frame.
                                                        // All other text container elements are different ...derived element type
        if(CapObj)
            CapObj->Text = CaptionText;
        return ;
    }
    auto Cap = new Label(this, CaptionText);
    Cap->Class = WClass::FrameComponent|WClass::Caption;
    ///@todo Implement positioning policies...
    Cap->SetPosition({Width() - Cap->Width()-2,0}); ///< Arbitrary right-justified position
    Cap->Attr = Char(Attr).SetBgFg(AttrDB::DB["Caption"][State::Active]).M;
    Cap->Attr |= Char::Underline;
    CaptionLabel = Cap;
}



} // Book::Ui
