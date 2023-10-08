    #include "devtest.h"



namespace book
{

devtest::~devtest()
{
}

devtest::devtest(const std::string& test_name)
{
}

book::code devtest::option_1(const cmd::argdata<devtest>& arg)
{
    //book::code::push_message(HERE) << "arguments:" << book::code::commit;
    for (auto const& str : arg.arguments)
    {
        //...
        //book::code::out() << str << book::code::commit;
        //...
    }
    return book::code::accepted;
}

book::code devtest::option_2(const cmd::argdata<devtest>& arg)
{
    //book::code::push_message(HERE) << "arguments:" << book::code::commit;
    for (auto const& str : arg.arguments)
    {
        //...
        //book::code::out() << str << book::code::commit;
        //...
    }
    return book::code::accepted;
}

book::code devtest::option_3(const cmd::argdata<devtest>& arg)
{
    //book::code::push_message(HERE) << "arguments:" << book::code::commit;
    for (auto const& str : arg.arguments)
    {
        //...
        //book::code::out() << str << book::code::commit;
        //...
    }
    return book::code::accepted;
}

book::code devtest::default_args(const cmd::argdata<devtest>& arg)
{
    //book::code::push_message(HERE) << "arguments:" << book::code::commit;
    for (auto const& str : arg.arguments)
    {
        //...
        //book::code::out() << str << book::code::commit;
        //...
    }

    return book::code::accepted;
}

book::code devtest::setup_cmdline_args(int argc, char** argv)
{
    cmdargs << cmd::argdata<devtest>{this, &devtest::option_1,"Expression", "-e", "--expression", "evaluate expression from the command line.", 1};
    cmd::argdata<devtest>& b = cmdargs << cmd::argdata<devtest>{this, &devtest::option_2,"Source File", "-f", "--source", "source file.", 1 };
    cmd::argdata<devtest>& c = cmdargs << cmd::argdata<devtest>{this, &devtest::option_3,"Source Files", "", "", "source files."};

    cmdargs.set_default_callback(&devtest::default_args);

    //book::code::push_debug(HERE) << " usage:" << book::code::commit;
    //book::code::out() << cmdargs.usage() << book::code::commit;

    //book::code::push_test() << " -- Now the exec: " << book::code::commit;

    return cmdargs.process(argc, argv);
}


}




auto main(int argc, char** argv) -> int
{
    book::devtest test;
    test.setup_cmdline_args(argc, argv);
    Book& AppBook = Book::init("logbook");
    AppBook.open();

    try {
        auto & lbdev  = AppBook.create_section("logbook.dev");
        std::cout << "\\O/ - " <<  lbdev.id() << " has been  successfuly created !!\n";
        lbdev.open();
        auto& bstack = lbdev.create_stack("stackdev");
        std::cout << " Prepare to use the instance of the bloc 'stackdev ( intentionnaly mispelled hihihihihi!@) ...:\n";
        auto & stackdev = AppBook["logbook.dev"]["stckdev"];


        lbdev.close();
    }
    catch(Book::exception e)
    {
        std::cout << "Book::exception chatched:\n[" << e.what() << "]\n";
    }

    AppBook.close();
    return 0;
}
