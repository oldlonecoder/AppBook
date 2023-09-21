#pragma once

#include <logbook/notify.h>



namespace book::cmd
{


struct BOOK_PUBLIC argdata
{
    using                  data = std::vector<std::string_view>;
    using list = std::vector<argdata>;
    using iterator = argdata::list::iterator;

    std::string            name;
    std::string            sw_char;
    std::string            sw_text;
    std::string            description;
    int                    required { 0 };
    int                    count    { 0 };

    notify<const argdata&> callback;
    data                   arguments{};
    bool                   uses{ false };

    bool operator !();
};


class BOOK_PUBLIC cargs
{
    argdata::list args;
    argdata defaults{ "defaults", "","", "default(orphan) arguments that have no switch",65000,0};

public:
    cargs() = default;
    ~cargs();
    argdata::iterator query_switch(const std::string& sw);
    argdata& operator << (argdata&& a);

    rem::code process(int argc, char** argv);
    rem::code set_default_callback(notify<const argdata&>::slot slot);
    std::string usage();

};

}