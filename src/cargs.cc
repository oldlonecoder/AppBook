#include "logbook/cargs.h"
namespace book::cmd 
{



cargs::~cargs()
{
    args.clear();
}

argdata::iterator cargs::query_switch(const std::string& sw)
{
    for (auto i = args.begin(); i != args.end(); i++)
    {
        if (sw == i->sw_text) return i;
        if (sw == i->sw_char) return i;
    }
    return args.end();
}

argdata& cargs::operator<<(argdata&& a)
{
    args.emplace_back(std::move(a));
    return args.back();
}

rem::code cargs::process(int argc, char** argv)
{
    argdata::iterator arg = args.end();
    for (int i = 1; i < argc; i++)
    {
        auto const* carg = argv[i];
        auto next_arg = query_switch(carg);
        if(next_arg == args.end())
        {
            if ((arg->required > arg->count) && (arg->required > 0)) // first pass it is the same as null_arg then use the defaults args
            {
                rem::push_debug(HERE) << " adding argument '" << color::Yellow << carg << color::Reset << "' to '" << color::LighcoreateBlue << arg->name << '\'' << rem::commit;
                arg->arguments.push_back(carg);
                ++arg->count;
                arg->uses = true;
            }
            else
            {
                rem::push_debug(HERE) << " adding argument '" << color::Yellow << carg << color::Reset << "' to '" << color::LighcoreateBlue << "defaults"  << '\'' << rem::commit;
                defaults.arguments.push_back(carg);
                defaults.uses = true;
            }
            continue;
        }
        else
            arg = next_arg;
    }

    rem::push_debug(HERE) << " Executing callbacks:" << rem::commit;
    for(auto& arg: args)
    {
        if(arg.uses)
            (void)arg.callback(arg);
    }

    return rem::notimplemented;
}

bool argdata::operator !()
{
    return uses == false;
}

std::string cargs::usage()
{
    stracc str;
    str << "usage:\n";
    str << "------------------------------------------------------\n";
    for (auto const& arg : args)
    {
        str << "%-2s | %-15s | %s" << arg.sw_char << arg.sw_text << arg.description << "\n";
    }
    return str();
}

}
