    #include "devtest.h"



namespace book
{

devtest::~devtest()
{
}

devtest::devtest(const std::string& test_name)
{
}

rem::code devtest::option_1(const cmd::argdata<devtest>& arg)
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

rem::code devtest::option_2(const cmd::argdata<devtest>& arg)
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

rem::code devtest::option_3(const cmd::argdata<devtest>& arg)
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

rem::code devtest::default_args(const cmd::argdata<devtest>& arg)
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
    cmdargs << cmd::argdata<devtest>{this, &devtest::option_1,"Expression", "-e", "--expression", "evaluate expression from the command line.", 1};
    cmd::argdata<devtest>& b = cmdargs << cmd::argdata<devtest>{this, &devtest::option_2,"Source File", "-f", "--source", "source file.", 1 };
    cmd::argdata<devtest>& c = cmdargs << cmd::argdata<devtest>{this, &devtest::option_3,"Source Files", "", "", "source files."};

    cmdargs.set_default_callback(&devtest::default_args);

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
