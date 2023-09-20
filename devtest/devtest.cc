#include "devtest.h"



namespace book
{

devtest::~devtest()
{
}

devtest::devtest(const std::string& test_name)
{
}

expect<> devtest::option_1(const cmd::argdata& arg)
{
    return expect<>();
}

expect<> devtest::option_2(const cmd::argdata& arg)
{
    return expect<>();
}

rem::code devtest::setup_cmdline_args(int argc, char** argv)
{
    cmdargs << cmd::argdata{"Expression", "-e", "--expression", "uses provided expression.", 1};

    return rem::notimplemented;
}


}




auto main(int argc, char** argv) -> int
{
    book::devtest test;
    test.setup_cmdline_args(argc, argv);

    return 0;
}
