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

#include <AppBook/Util/Delegate.h>
#include <AppBook/Book/BookEnums.h>



namespace Cmd
{


struct APPBOOK_EXPORTS Switch
{
    using Data = std::vector<std::string_view>;
    using Array = std::vector<Switch*>;
    using Iterator = Switch::Array::iterator;

// ----------  ------------------------------------------
    std::string            Name;
    std::string            SwitchChar;
    std::string            SwitchText;
    std::string            Description;
    int                    Required { 0 };
    int                    Count    { 0 };
//--------------------------------------------------------


    Data                   Arguments{};
    bool                   Enabled{ false };
    Delegate<Switch&> DelegateCB;

    bool operator !() { return DelegateCB.Empty(); }

    template<typename T> void Connect(T* Obj, Book::Enums::Action (T::*Fn)(Switch&)){
        DelegateCB.Connect(Obj, Fn);
    }
    ~Switch();
};


class APPBOOK_EXPORTS CArgs
{

    Switch::Array Args;
    Switch Defaults;

    Switch::Iterator A{};
public:
    CArgs() = default;
    ~CArgs();

    template<typename T> void SetDefaultCallback(T* Obj, Book::Enums::Code (T::*Fn)(Switch&))
    {
        (void)Defaults.DelegateCB.Connect(Obj, Fn);
    }

    Switch::Iterator Query(std::string_view Switch);
    Switch& operator<<(const Switch& Arg);
    Switch& operator [] (const std::string& ArgName);
    //Book::Enums::Code InputCmdLineData(int argc, char** argv);
    Book::Enums::Code Input(const std::vector<std::string_view>& StrArray);

    Book::Action Process();

    std::string Usage();
};

}
