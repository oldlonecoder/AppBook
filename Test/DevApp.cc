#include "DevApp.h"
//#include <source_location>
#include "AppBook/Book/StmlText.h"
#include "AppBook/Util/StrBreak.h"
#include <AppBook/Utf/Cadres.h>
#include <AppBook/Console/Console.h>
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
    AppBook::Close();
}

DevApp::DevApp(const std::string& TestName, int argc, char** argv){
    for (int i = 1; i < argc; i++)
        inArgs.emplace_back(argv[i]);
}



Book::Result DevApp::SetupArgs()
{
    (Args << Cmd::Switch{"StrBreak test",  "-w", "--Break","Test Book::StrBreak", 1 }).Connect(this, &DevApp::StrBreakTest);
    (Args << Cmd::Switch{"Test TScanner",  "-s",  "--TScanner", "Test TScanner", 0 }).Connect(this, &DevApp::SVScanTest);
    (Args << Cmd::Switch{"Dummy test",     "-d", "--Dummy", "Dummy[test]",0 }).Connect(this, &DevApp::DummyTest);
    (Args << Cmd::Switch{"CWindow test",   "-n", "--ConsoleWindow", "Console/CWindow and Utf::Cadre tests",0 }).Connect(this, &DevApp::ConsoleWindowTest);

    return Args.Input(inArgs);
}

Book::Result DevApp::Run()
{


    (void)Args.Process();
    return Book::Result::Ok;
}

Book::Action DevApp::StrBreakTest(Cmd::Switch& arg)
{
    AppBook::Message() << "Arguments:";

    for (auto const& str : arg.Arguments)
    {
        //...
        AppBook::Out() << str;
        //...
    }
    AppBook::Info() << " Processing argument:" << Color::Yellow << arg.Arguments[0];

    StrBreak::ConfigData Data = {arg.Arguments[0],"/", StrBreak::Keep};
    StrBreak Sb;
    auto C = Sb(Data);
    for(auto const& S : Data.Words)
    {
        AppBook::Out() << S();
    }
    Data.Words.clear();
    return Book::Action::Continue;
}

Book::Action DevApp::SVScanTest(Cmd::Switch& arg)
{
    auto view = R"(
        x'777'777;
)"sv;

    AppBook::Test() << " Testing Processing::ScanNumber: R\"(" << Color::Yellow << view << Color::Reset << ")\"sv;";

    Book::TScanner Text{view};

    Text.SkipWS();
    std::string Str;
    Text.Sync() >> Str;
    AppBook::Debug() << " Text.Sync(): " << Str;
    auto [Rn, Details] = Text.ScanNumber();
     

    if(Rn == Book::Result::Accepted)
    {
        AppBook::Debug() << " Text.ScanNumber - Sequence:[" << Color::Yellow << std::string(Details.Seq.begin(), Details.Seq.end()) << Color::Reset << "] :" << Color::Yellow << (int)Details.Value;
        if(Details.Base == Book::TScanner::Numeric::Details::BaseSize::Binary)
            AppBook::Out() << StrAcc::ToBinary((int16_t)Details.Value, true, 4);
    }
    else
        AppBook::Debug() << "Text.ScanNumber: " << Book::Result::Failed;

    Text = R"(
        "This world has become an open-wide planetary \"psychiatric hospital\" managed by the fouls!"
)"sv;

    Text.SkipWS();
    auto [R,vv] = Text.ScanLiteralString();
    AppBook::Test() << " Processing::ScanLiteralString() : '" << Color::Lime <<  std::string(vv.begin(), vv.end()) << Color::Reset << "' ";
    //...
    return Book::Action::Continue;
}

Book::Action DevApp::DummyTest(Cmd::Switch& arg)
{
    AppBook::Message() << " Args:";
    for(auto const& A: arg.Arguments)
    {
        AppBook::Out() << A;
    }
    //...
    return Book::Action::Continue;
}

Book::Action DevApp::Defaults(Cmd::Switch &arg)
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

Book::Action DevApp::ConsoleWindowTest(Cmd::Switch &arg)
{
    AppBook::Message() << " Args:";
    for(auto const& A: arg.Arguments)
    {
        AppBook::Out() << A;
    }
    R = {Point(2,2),Dim(140,12)};
    Book::ConIO::CWindow Window(nullptr,"Test Window");
    Window.SetGeometry(R.Dwh);
    Window.Alloc();

    auto& Pen = Window.BeginWrite(Rect(Point{1,1},Dim{138,11}));
    Pen.Position({3,3});
    //Pen.Clear(Color::Navy);
    Pen << Color::LightGreen << Utf::Glyph::World
    << Color::Yellow6 << "Hello "
    << Color::Pair(Color::Grey100,Color::Reset)
    << "World!!!";

    Window.EndWrite(Pen);
    Window.DrawFrame();
    StrAcc Text;
    Window >> Text;
    Book::Debug() << " Utf::CWindow :";
    Book::Out() << Text;
    Window.Draw();
    return Book::Action::Leave;
}


int main(int argc, char** argv)
{

    std::signal(SIGINT, sig_int);
    std::signal(SIGSEGV, sig_fault);
    std::signal(SIGABRT, sig_abort);

    AppBook& Livre = AppBook::Open("ApplicationBase");
    Livre.Descriptions =
R"(
<Icon:School; fg:Yellow>This is the development of the <fg:White>[<fg:Lime>ApplicationBase <fg:LightCyan3>Book <fg:BlueViolet>API<fg:White>]<fg/>
--------------------------------------------------------
)";

    std::string head;
    Book::STMLText MLText;
    DevApp test = { "AppBook:Tests", argc,argv };
    try {
        auto & LivreSection  = AppBook::CreateSection("AppBook.Dev");
        LivreSection.Open().CreateSectionContents("TestStml");
        /*auto & stackdev =*/
        Livre["AppBook.Dev"]["TestStml"];

        Book::Enums::Code c = MLText << Livre.Descriptions >> head;
        if(c != Book::Enums::Code::Success)
            std::cout << " main: the description text failed to compile: " << Book::Enums::CodeText(c) << "\n";
        else
            AppBook::Out() << Book::Enums::Fn::Weekday << ' ' << Book::Enums::Fn::Stamp << Book::Enums::Fn::Endl << head;

        auto FrameChar = Utf::Cadre()[Utf::Cadre::Cross];

        Book::Test() << " StringLength of Frame : TopLeft [" << Color::Yellow << FrameChar << Color::Reset << "]: " << FrameChar.length();
        Book::Debug() << "Sizeof the Frame components MAP: " << Utf::Cadre::FrameMatrix.size();
        AppBook::Info() << "Last line, committing Section::Contents and closing the book:";
        AppBook::Out() << "-----------------------------------------------------------------";
        
        test.SetupArgs();
        test.Run();
        
        AppBook::Commit();
    }
    catch(AppBook::Exception& e)
    {
        //
        AppBook::Out() << Color::Reset << test.Arguments().Usage();
    }
    //catch(std::exception& e)
    //{
    //    std::cout << "\n\nstd::exception caught:\n[" << Color::Ansi(Color::LightCoral) << e.what() << Color::Ansi(Color::Reset) << "]\n\n";
    //}
    catch(const char* msg)
    {
        std::cout << "\n\n global exception caught:\n[" << Color::Ansi(Color::LightCoral) << msg << Color::Ansi(Color::Reset) << "]\n\n";
    }
    return 0;
}
