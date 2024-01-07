#include "DevApp.h"
//#include <source_location>
#include "AppBook/Book/StmlText.h"
#include "AppBook/Util/StrBreak.h"

#include <csignal>


void sig_int( int s )
{
    //rem::push_interrupted() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << "\n---------------------------------------\n sig interrupt caugh - flushing Book contents.\n ";
    AppBook::Close();
    exit(3);
}

void sig_fault( int s)
{
    //rem::push_segfault() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << " sigfault caught...\n";
    AppBook::Close();
    exit(127);
}

void sig_abort( int s)
{
    //rem::push_aborted() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << s << '\n';
    AppBook::Close();
    exit(127);
}




DevApp::~DevApp()
{

}

DevApp::DevApp(const std::string& test_name)
{
}

Book::Action DevApp::Opt1(Cmd::ArgumentData& arg)
{
    AppBook::Message() << "Arguments:";

    for (auto const& str : arg.Arguments)
    {
        //...
        AppBook::Out() << str;
        //...
    }
    StrBreak::ConfigData Data = {arg.Arguments[0],"/", StrBreak::Keep};
    StrBreak Sb;
    auto C = Sb(Data);
    for(auto S : Data.Words)
    {
        AppBook::Out() << S();
    }
    Data.Words.clear();

    return Book::Action::Continue;
}

Book::Action DevApp::Opt2(Cmd::ArgumentData& arg)
{
    //Book::Enums::Code::push_message(HERE) << "arguments:" << Book::Enums::Code::commit;
    for (auto const& str : arg.Arguments)
    {
        //...
        //Book::Enums::Code::out() << str << Book::Enums::Code::commit;
        //...
    }
    return Book::Action::Continue;
}

Book::Action DevApp::Opt3(Cmd::ArgumentData& arg)
{
    //Book::Enums::Code::push_message(HERE) << "arguments:" << Book::Enums::Code::commit;
    for (auto const& str : arg.Arguments)
    {
        //...
        //Book::Enums::Code::out() << str << Book::Enums::Code::commit;
        //...
    }
    return Book::Action::Continue;
}

Book::Action DevApp::Defaults(Cmd::ArgumentData &arg)
{
    //Book::Enums::Code::push_message(HERE) << "arguments:" << Book::Enums::Code::commit;
    for (auto const& str : arg.Arguments)
    {
        //...
        //Book::Enums::Code::out() << str << Book::Enums::Code::commit;
        //...
    }

    return Book::Action::Continue;
}

Book::Action DevApp::CmdArgs(int argc, char **argv)
{

    Args << Cmd::ArgumentData{"Stml test",       "-m","--Stml","Test Book::STMLText", 1};
    Args << Cmd::ArgumentData{"Expression test", "-e", "--expression", "Test expression [test/fake]", 1};
    Args << Cmd::ArgumentData{"Usage test",      "-u", "--help", "Display Usage [test]",0};

    Args["Stml test"].Connect(this, &DevApp::Opt1);

    AppBook::Out() << " usage:";
    AppBook::Info() << Args.Usage();
    Args.InputCmdLineData(argc, argv);

    //...

    return Args.Execute();
}



int main(int argc, char** argv)
{

    std::signal(SIGINT, sig_int);
    std::signal(SIGSEGV, sig_fault);
    std::signal(SIGABRT, sig_abort);

    AppBook& Livre = AppBook::Init("Application");
    Livre.Open();
    Livre.Descriptions =
R"(
<Icon:School; fg:Yellow>This is the development of the <fg:White>[Application Book API]<fg/>
--------------------------------------------------------
)";

    std::string head;

    Book::STMLText MLText;
    try {
        auto & LivreSection  = Livre.CreateSection("AppBook.Dev");
        LivreSection.Open().CreateSectionContents("TestStml");
        /*auto & stackdev =*/
        Livre["AppBook.Dev"]["TestStml"];

        Book::Enums::Code c = MLText << Livre.Descriptions >> head;
        if(c != Book::Enums::Code::Success)
            std::cout << " main: the description text failed to compile: " << Book::Enums::CodeText(c) << "\n";
        else
            AppBook::Out() << Book::Enums::Fn::Weekday << ' ' << Book::Enums::Fn::Stamp << Book::Enums::Fn::Endl << head;


        AppBook::Info() << "Last line, committing Section::Contents and closing the book:";
        AppBook::Out() << "-----------------------------------------------------------------";
        DevApp test;
        test.CmdArgs(argc, argv);
        AppBook::Commit();
    }
    catch(AppBook::Exception& e)
    {
        std::cout << "Book::exception caugh:\n[" << e.what() << "]\n";
    }

    AppBook::Close();

    return 0;
}
