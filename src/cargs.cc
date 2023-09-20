#include "logbook/cargs.h"
namespace book::cmd 
{

argdata null_arg;

cargs::~cargs()
{
    args.clear();
}

argdata& cargs::query_switch(const std::string& sw)
{
    for (auto& a : args)
    {
        if (sw == a.sw_text) return a;
        if (sw == a.sw_char) return a;
    }
    return null_arg;
}

argdata& cargs::operator<<(argdata&& a)
{
    args.emplace_back(std::move(a));
    return args.back();
}




bool argdata::operator!()
{
    return sw_char.empty() || sw_text.empty();
}

rem::code cargs::process(int argc, char** argv)
{
    argdata& arg = null_arg; // Init with dummy argdata 

    for (int i = 1; i < argc; i++)
    {
        auto const* carg = argv[i];
        auto & next_arg = query_switch(carg);
        if(!next_arg)
        {
            if ((arg.required > arg.count) && (arg.required > 0)) // first pass it is the same as null_arg then use the defaults args
            {
                arg.arguments.push_back(carg);
                ++arg.count;
            }
            else
                defaults.arguments.push_back(carg);
            continue;
        }
        else
            arg = next_arg;   
    }
    return rem::notimplemented;
}

}
