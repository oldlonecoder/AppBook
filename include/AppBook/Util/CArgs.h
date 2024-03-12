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


struct APPBOOK_EXPORTS ArgumentData
{
    using Data = std::vector<std::string_view>;
    using Array = std::vector<ArgumentData*>;
    using Iterator = ArgumentData::Array::iterator;

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
    Delegate<ArgumentData&> DelegateCB;

    bool operator !() { return DelegateCB.Empty(); }

    template<typename T> void Connect(T* Obj, Book::Enums::Action (T::*Fn)(ArgumentData&)){
        DelegateCB.Connect(Obj, Fn);
    }
    ~ArgumentData();
};


class APPBOOK_EXPORTS CArgs
{

    ArgumentData::Array Args;
    ArgumentData Defaults;

public:
    CArgs() = default;
    ~CArgs();

    template<typename T> void SetDefaultCallback(T* Obj, Book::Enums::Code (T::*Fn)(ArgumentData&))
    {
        (void)Defaults.DelegateCB.Connect(Obj, Fn);
    }

    ArgumentData::Iterator Query(const std::string& Switch);
    ArgumentData& operator<<(const ArgumentData& Arg);
    ArgumentData& operator [] (const std::string& ArgName);
    Book::Enums::Code InputCmdLineData(int argc, char** argv);
    Book::Enums::Code ProcessStringArray(std::vector<std::string_view> StrArray);

    Book::Action Execute();

    std::string Usage();
};

}
