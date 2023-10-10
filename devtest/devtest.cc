#include "devtest.h"
#include <source_location>
#include <logbook/text.h>
#include <signal.h>


void sig_int( int s )
{
    //rem::push_interrupted() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << "\n---------------------------------------\n sig interrupt caugh - flushing Book contents.\n ";
    Book::close();
    exit(3);
}

void sig_fault( int s)
{
    //rem::push_segfault() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << " sigfault caught...\n";
    Book::close();
    exit(127);
}

void sig_abort( int s)
{
    //rem::push_aborted() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << s << '\n';
    Book::close();
    exit(127);
}



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

    ::signal(SIGINT, sig_int);
    ::signal(SIGSEGV, sig_fault);
    ::signal(SIGABRT, sig_abort);
    book::devtest test;
    test.setup_cmdline_args(argc, argv);
    Book& AppBook = Book::init("logbook");
    AppBook.open();
    AppBook.descriptions =
R"(<Icon:School; fg:Yellow>Application Book API development:<fg/>
----------------------------------------
)";

    std::string head;

    book::text description;
    try {
        auto & lbdev  = AppBook.create_section("logbook.dev");
        lbdev.open().create_stack("text-processor-dev");
        /*auto & stackdev =*/
        AppBook["logbook.dev"]["text-processor-dev"];

        book::code c = description << AppBook.descriptions >> head;
        if(c != book::code::success)
            std::cout << " main: the description text failed to compile: " << book::code_text(c) << "\n";
        else
            Book::out() << head;

        Book::out() << "Last line, committing blocstack and closing the book:";
        Book::commit();
    }
    catch(Book::exception e)
    {
        std::cout << "Book::exception caugh:\n[" << e.what() << "]\n";
    }

    AppBook.close();
    return 0;
}
