#include "devtest.h"



namespace book
{

devtest::~devtest()
{
}

devtest::devtest(const std::string& test_name)
{
}

expect<> devtest::option_1(const cmd::argdata& arg)
{
    rem::push_message(HERE) << "arguments:" << rem::commit;
    for (auto const& str : arg.arguments)
    {
        //...
        rem::out() << str << rem::commit;
        //...
    }
    return rem::accepted;
}

expect<> devtest::option_2(const cmd::argdata& arg)
{
    rem::push_message(HERE) << "arguments:" << rem::commit;
    for (auto const& str : arg.arguments)
    {
        //...
        rem::out() << str << rem::commit;
        //...
    }
    return rem::accepted;
}

expect<> devtest::option_3(const cmd::argdata& arg)
{
    rem::push_message(HERE) << "arguments:" << rem::commit;
    for (auto const& str : arg.arguments)
    {
        //...
        rem::out() << str << rem::commit;
        //...
    }
    return rem::accepted;
}

expect<> devtest::default_args(const cmd::argdata& arg)
{
    rem::push_message(HERE) << "arguments:" << rem::commit;
    for (auto const& str : arg.arguments)
    {
        //...
        rem::out() << str << rem::commit;
        //...
    }

    return rem::accepted;
}

rem::code devtest::setup_cmdline_args(int argc, char** argv)
{
    cmd::argdata& a = cmdargs << cmd::argdata{"Expression", "-e", "--expression", "evaluate expression from the command line.", 1};
    a.callback.connect(this, &devtest::option_1);
    cmd::argdata& b = cmdargs << cmd::argdata{"Source File", "-f", "--source", "source file.", 1 };
    b.callback.connect(this, &devtest::option_2);
    cmd::argdata& c = cmdargs << cmd::argdata{"Source Files", "", "", "source files."};
    c.callback.connect(this, &devtest::option_3);

    cmdargs.set_default_callback([this](const cmd::argdata& a)->expect<> { return default_args(a); });
    
    rem::push_debug(HERE) << " usage:" << rem::commit;
    rem::out() << cmdargs.usage() << rem::commit;

    rem::push_test() << " -- Now the exec: " << rem::commit;

    return cmdargs.process(argc, argv);
}


}




auto main(int argc, char** argv) -> int
{
    book::devtest test;
    test.setup_cmdline_args(argc, argv);

    return 0;
}
