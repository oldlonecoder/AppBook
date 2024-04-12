#pragma once

#include <AppBook/Book/TScanner.h>
#include <AppBook/Util/CArgs.h>
#include <AppBook/Console/Window.h>
#include <AppBook/Utf/Cadres.h>
class DevApp
{

    Cmd::CArgs Args;
    std::vector<std::string_view> inArgs{};

    Rect R{};
    Utf::Cadre  Cadre;
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
    Book::Action ConsoleWindowTest(Cmd::Switch& arg);
    Cmd::CArgs Arguments() { return Args; }


};


