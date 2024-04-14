//
// Created by oldlonecoder on 24-04-14.
//

#include <AppBook/ConIO/UiEnums.h>



namespace Book::Ui
{

AttrDB::Elements AttrDB::DB =
{
    {"Element",{
            {State::Active, {Color::Yellow, Color::DarkBlue}},
            {State::Normal, {Color::Grey0, Color::DarkBlue}},
            //...
        }
    },
    {"Frame", {
        {State::Active, {Color::Grey100, Color::DarkBlue}},
        {State::Normal, {Color::Grey0, Color::DarkBlue}},
        //...
        }
    }
};


}