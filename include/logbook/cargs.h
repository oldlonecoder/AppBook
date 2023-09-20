#pragma once

#include <logbook/notify.h>



namespace book::cmd
{


struct BOOK_PUBLIC argdata
{
    std::string sw_char;
    std::string sw_text;
    std::string name;

    int         required { 0 };
    int         count    { 0 };
    notify<const argdata&> callback;


    using list = std::vector<argdata>;
    using iterator = argdata::list::iterator;

    bool operator !();
};

class BOOK_PUBLIC cargs
{
    argdata::list args;

public:
    cargs() = default;
    ~cargs();
    argdata& query(const std::string& sw);
    argdata& operator << (argdata&& a);

};

}