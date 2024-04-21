//
// Created by oldlonecoder on 24-04-14.
//

#include <AppBook/Ui/UiEnums.h>



namespace Book::Ui
{

AttrDB::Elements AttrDB::DB =
{
    {"Element",
        {
            {State::Active, {Color::Yellow, Color::DarkBlue}},
            {State::Normal, {Color::Grey0, Color::DarkBlue}},
            //...
        }
    },
    {"Frame",
        {
            {State::Active, {Color::Grey100, Color::DarkBlue}},
            {State::Normal, {Color::Grey0, Color::DarkBlue}},
        //...
        }
    },
    {"Glyph",
        {
            {State::Active, {Color::Maroon,Color::DarkBlue}},
            {State::Normal, {Color::Maroon,Color::DarkBlue}},
        //...
        }
    },
    {"Caption",
        {
            {State::Active, {Color::Grey78,Color::DarkBlue}},
            {State::Normal, {Color::DarkCyan,Color::DarkBlue}},
        //...
        }
    },
    {"InputField", // Including underline attr
        {
            {State::Active, {Color::LighcoreateBlue,Color::Reset}},
            {State::Normal, {Color::White,Color::Reset}},
            {State::Error,  {Color::Red,Color::Reset}},
            //...
        }
    },
};


}