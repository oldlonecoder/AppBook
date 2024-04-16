//
// Created by oldlonecoder on 24-04-14.
//

#include <AppBook/ConIO/UiEnums.h>



namespace Book::Ui
{

AttrDB::Elements AttrDB::DB =
{
    {"Element",
        {
            {State::Active, {Color::Yellow, Color::Blue}},
            {State::Normal, {Color::Grey0, Color::Blue}},
            //...
        }
    },
    {"Frame",
        {
            {State::Active, {Color::Grey100, Color::Blue}},
            {State::Normal, {Color::Grey0, Color::Blue}},
        //...
        }
    },
    {"Glyph",
        {
            {State::Active, {Color::Maroon,Color::Blue}},
            {State::Normal, {Color::Maroon,Color::Blue}},
        //...
        }
    },
    {"Caption",
        {
            {State::Active, {Color::GreenYellow,Color::Blue}},
            {State::Normal, {Color::DarkCyan,Color::Blue}},
        //...
        }
    }
};


}