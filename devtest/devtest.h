#pragma once


#include <logbook/cargs.h> // { notify, expect, rem, text  }
#include <logbook/Book.h>


namespace book
{

class devtest
{
    cmd::cargs<devtest> cmdargs;
    std::string name;
    Book* AppBook{nullptr};

public:

    devtest() = default;
    ~devtest();
    devtest(const std::string& test_name);

    book::code option_1(const cmd::argdata<devtest>& arg);
    book::code option_2(const cmd::argdata<devtest>& arg);
    book::code option_3(const cmd::argdata<devtest>& arg);
    book::code default_args(const cmd::argdata<devtest>& arg);
    book::code setup_cmdline_args(int argc, char **argv);
};

}
