#pragma once

#include "AppBook/Book/SVScanner.h"
#include <AppBook/Util/CArgs.h>
class DevApp
{

    Cmd::CArgs Args;
    std::vector<std::string_view> inArgs{};
public:

    DevApp() = default;
    ~DevApp();
    DevApp(const std::string& TestName, int argc, char** argv);


    Book::Result SetupArgs();
    Book::Result Run();

    Book::Action StrBreakTest(Cmd::ArgumentData& arg);
    Book::Action SVScanTest(Cmd::ArgumentData& arg);
    Book::Action DummyTest(Cmd::ArgumentData& arg);
    Book::Action Defaults(Cmd::ArgumentData& arg);
    
    Cmd::CArgs Arguments() { return Args; }
};


