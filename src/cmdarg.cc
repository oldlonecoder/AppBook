#include "logbook/cmdarg.h"


namespace book::cmdline
{


param null_arg{};


book::rem::code cmdargs::process(int argc, char** argv)
{
    for (int a = 1; a < argc; a++)
    {
        for (auto& p : args)
        {
            
        }
    }
}

cmdargs::~cmdargs()
{
}

param& cmdargs::query(const std::string& argswitch)
{
    // TODO: insérer une instruction return ici
}

param& cmdargs::operator[](const std::string& argswitch)
{

    for (auto& a : args)
        if (argswitch == a.switch_string)
            return a;

    return null_arg;
}

param& cmdargs::operator<<(book::cmdline::param&& a)
{
    args.emplace_back(std::move(a));
    return args.back();
}





}
