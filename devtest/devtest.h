#pragma once


#include <logbook/cargs.h> // { notify, expect, rem, text  }


namespace book
{

class devtest
{
    cmd::cargs<devtest> cmdargs;
    std::string name;
public:

    devtest() = default;
    ~devtest();
    devtest(const std::string& test_name);

    rem::code option_1(const cmd::argdata<devtest>& arg);
    rem::code option_2(const cmd::argdata<devtest>& arg);
    rem::code option_3(const cmd::argdata<devtest>& arg);
    rem::code default_args(const cmd::argdata<devtest>& arg);
    book::rem::code setup_cmdline_args(int argc, char **argv);
};

}
