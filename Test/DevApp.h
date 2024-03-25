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

    Book::Action StrBreakTest(Cmd::Switch& arg);
    Book::Action SVScanTest(Cmd::Switch& arg);
    Book::Action DummyTest(Cmd::Switch& arg);
    Book::Action Defaults(Cmd::Switch& arg);
    
    Cmd::CArgs Arguments() { return Args; }
};


