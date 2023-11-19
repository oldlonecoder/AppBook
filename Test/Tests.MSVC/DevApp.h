#pragma once


#include "AppBook/Book/AppBook.h" // { notify, Expect, rem, text  }
#include "AppBook/Util/CArgs.h"




class DevApp
{
    Cmd::CArgs Args;
    std::string Name;

public:

    DevApp() = default;
    ~DevApp();
    DevApp(const std::string& test_name);

    Book::Enums::Code Opt1(Cmd::ArgumentData& arg);
    Book::Enums::Code Opt2(Cmd::ArgumentData& arg);
    Book::Enums::Code Opt3(Cmd::ArgumentData& arg);
    Book::Enums::Code Defaults(Cmd::ArgumentData& arg);
    Book::Enums::Code CmdArgs(int argc, char **argv);
};


