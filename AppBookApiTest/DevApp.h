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

    Book::Action Opt1(Cmd::ArgumentData& arg);
    Book::Action Opt2(Cmd::ArgumentData& arg);
    Book::Action Opt3(Cmd::ArgumentData& arg);
    Book::Action Defaults(Cmd::ArgumentData& arg);
    Book::Action CmdArgs(int argc, char **argv);
};


