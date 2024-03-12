#pragma once


#include "AppBook/Book/Tests/Test.h"

#include "AppBook/Book/SVScanner.h"

class DevApp : public Book::Test
{

    Cmd::CArgs Args;
public:

    DevApp() = default;
    ~DevApp() override;
    DevApp(const std::string& TestName);

    Book::Result Run() override { return Book::Result::Notimplemented; }

    Book::Action StrBreakTest(Cmd::ArgumentData& arg);
    Book::Action SVScanTest(Cmd::ArgumentData& arg);
    Book::Action DummyTest(Cmd::ArgumentData& arg);
    Book::Action Defaults(Cmd::ArgumentData& arg);
    Book::Action CmdArgs(int argc, char **argv);
};


