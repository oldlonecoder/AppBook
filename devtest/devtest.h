#pragma once


#include <logbook/cargs.h> // { notify, expect, rem, text  }


namespace book
{

class devtest
{
    cmd::cargs cmdargs;
    std::string name;
public:

    devtest() = default;
    ~devtest();
    devtest(const std::string& test_name);

    expect<> option_1(const cmd::argdata& arg);
    expect<> option_2(const cmd::argdata& arg);

    book::rem::code setup_cmdline_args(int argc, char **argv);
};

}
