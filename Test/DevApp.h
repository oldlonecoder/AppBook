#pragma once


#include "AppBook/Book/AppBook.h" // { notify, expect, rem, text  }
#include "AppBook/Core/CArgs.h"




class DevApp
{
    Core::Cmd::CArgs Args;
    std::string Name;

public:

    DevApp() = default;
    ~DevApp();
    DevApp(const std::string& test_name);

    Book::Enums::Code Opt1(Core::Cmd::ArgumentData& arg);
    Book::Enums::Code Opt2(Core::Cmd::ArgumentData& arg);
    Book::Enums::Code Opt3(Core::Cmd::ArgumentData& arg);
    Book::Enums::Code Defaults(Core::Cmd::ArgumentData& arg);
    Book::Enums::Code CmdArgs(int argc, char **argv);
};


