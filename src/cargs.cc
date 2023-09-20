#include "logbook/cargs.h"
namespace book::cmd 
{

argdata null_arg;

book::cmd::cargs::~cargs()
{
    args.clear();
}

argdata& book::cmd::cargs::query(const std::string& sw)
{
    for (auto& a : args)
    {
        if (sw == a.sw_text) return a;
        if (sw == a.sw_char) return a;
    }
    return null_arg;
}

argdata& book::cmd::cargs::operator<<(argdata&& a)
{
    args.emplace_back(std::move(a));
    return args.back();
}




bool book::cmd::argdata::operator!()
{
    return sw_char.empty() || sw_text.empty();
}


}
