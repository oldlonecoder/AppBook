#pragma once

#include "AppBook/Book/SVScanner.h"
#include <AppBook/Util/CArgs.h>
class DevApp
{

    Cmd::CArgs Args;
public:

    DevApp() = default;
    ~DevApp();
    DevApp(const std::string& TestName);



    Book::Action StrBreakTest(Cmd::ArgumentData& arg);
    Book::Action SVScanTest(Cmd::ArgumentData& arg);
    Book::Action DummyTest(Cmd::ArgumentData& arg);
    Book::Action Defaults(Cmd::ArgumentData& arg);
    Book::Action CmdArgs(int argc, char **argv);
};


