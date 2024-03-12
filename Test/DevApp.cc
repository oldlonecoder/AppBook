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

DevApp::DevApp(const std::string& TestName):Test(nullptr, TestName) {}





Book::Action DevApp::StrBreakTest(Cmd::ArgumentData& arg)
{
    AppBook::Message() << "Arguments:";

    for (auto const& str : arg.Arguments)
    {
        //...
        AppBook::Out() << str;
        //...
    }
    AppBook::Info() << " Processing argument:";

    StrBreak::ConfigData Data = {arg.Arguments[0],"/", StrBreak::Keep};
    StrBreak Sb;
    auto C = Sb(Data);
    for(auto const& S : Data.Words)
    {
        AppBook::Out() << S();
    }
    Data.Words.clear();
    Passed();
    Failed();
    return Book::Action::Continue;
}

Book::Action DevApp::SVScanTest(Cmd::ArgumentData& arg)
{
    auto view = R"(
        o777777;
)"sv;

    AppBook::Test() << " Testing TextProc::ScanNumber: R\"(" << Color::Yellow << view << Color::Reset << ")\"sv;";

    Book::SVScanner Text{view};

    Text.SkipWS();
    std::string Str;
    Text.Sync() >> Str;
    AppBook::Debug() << " Text.Sync(): " << Str;
    auto [Rn, Details] = Text.ScanNumber();
     

    if(Rn == Book::Result::Accepted)
    {
        AppBook::Debug() << " Text.ScanNumber - Sequence:[" << Color::Yellow << std::string(Details.Seq.begin(), Details.Seq.end()) << Color::Reset << "] :" << Color::Yellow << (int)Details.Value;
        if(Details.Base == Book::SVScanner::Numeric::Details::BaseSize::Binary)
            AppBook::Out() << StrAcc::ToBinary((int16_t)Details.Value, true, 4);
    }
    else
        AppBook::Debug() << "Text.ScanNumber: " << Book::Result::Failed;

    Text = R"(
        "We Are the \" World"
)"sv;

    Text.SkipWS();
    auto [R,vv] = Text.ScanLiteralString();
    AppBook::Test() << " TextProc::ScanLiteralString() : '" << Color::Lime <<  std::string(vv.begin(), vv.end()) << Color::Reset << "' ";
    //...
    return Book::Action::Continue;
}

Book::Action DevApp::DummyTest(Cmd::ArgumentData& arg)
{
    AppBook::Message() << " Args:";
    for(auto const& A: arg.Arguments)
    {
        AppBook::Out() << A;
    }
    //...
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

    (Args << Cmd::ArgumentData{ "StrBreak test",   "-w", "--Break","Test Book::StrBreak", 2 }).Connect(this, &DevApp::StrBreakTest);
    (Args << Cmd::ArgumentData{ "Test SVScanner",  "-s",  "--SVScanner", "Test SVScanner", 0 }).Connect(this, &DevApp::SVScanTest);
    (Args << Cmd::ArgumentData{ "Dummy test",      "-d", "--Dummy", "Dummy[test]",0 }).Connect(this, &DevApp::DummyTest);



    Args.InputCmdLineData(argc, argv);
    //Args.ProcessStringArray(StrAcc::SVArray(argc,argv));

    //...

    auto R =  Args.Execute();
    AppBook::Out() << " usage:";
    AppBook::Info() << Args.Usage();
    return R;
}



int main(int argc, char** argv)
{

    std::signal(SIGINT, sig_int);
    std::signal(SIGSEGV, sig_fault);
    std::signal(SIGABRT, sig_abort);

    AppBook& Livre = AppBook::Open("Application");
    Livre.Descriptions =
R"(
<Icon:School; fg:Yellow>This is the development of the <fg:White>[<fg:Lime>Application <fg:LightCyan3>Book <fg:BlueViolet>API<fg:White>]<fg/>
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

        AppBook::Test() << Color::LightGreen << Utf::Glyph::Success << Color::Reset << " Passed!";

        AppBook::Info() << "Last line, committing Section::Contents and closing the book:";
        AppBook::Out() << "-----------------------------------------------------------------";
        DevApp test = {"STML"};

        test.CmdArgs(argc, argv);

        AppBook::Commit();
    }
    catch(AppBook::Exception& e)
    {
        std::cout << "Book::Exception caught:\n[" << e.what() << "]\n";
    }
    catch(std::exception& e)
    {
        std::cout << "\n\nstd::exception caught:\n[" << Color::Ansi(Color::LightCoral) << e.what() << Color::Ansi(Color::Reset) << "]\n\n";
    }
    catch(const char* msg)
    {
        std::cout << "\n\n global exception caught:\n[" << Color::Ansi(Color::LightCoral) << msg << Color::Ansi(Color::Reset) << "]\n\n";
        exit(1);
    }

    AppBook::Close();

    return 0;
}
