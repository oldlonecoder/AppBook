#pragma once

#include <logbook/notify.h>




namespace book::cmdline
{



struct BOOK_PUBLIC param
{
    using args = std::vector<std::string_view>;
    using list = std::vector<param>;
    using iterator = param::list::iterator;

    ::book::notify<param::args> proc; ///< processing callback on a given switch/arg

    std::string switch_string;
    char        switch_char{ 0 };
    std::string descriptions;
    size_t      args_req{ 0 };
    size_t      args_count{ 0 };

};

class BOOK_PUBLIC cmdargs
{
    param::list args;
    param::args arg_list;
    notify<param::args> default_cb;
public:
    cmdargs() = default;
    ~cmdargs();


    param& query(const std::string& argswitch);
    param& operator[](const std::string& argswitch);

    param& operator << (book::cmdline::param&& a);

    book::rem::code process(int argc, char** argv);
};

}

