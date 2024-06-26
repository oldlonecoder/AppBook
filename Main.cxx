//
// Created by oldlonecoder on 24-04-10.
//

#include <AppBook/Book/ApplicationBase.h>
#include <AppBook/Ui/Widget/Label.h>
#include <AppBook/Ui/Widget/Frame.h>

namespace Book
{


class Application : public Book::ApplicationBase {

public:

    Application(const std::string &appName, int argc, char **argv) : ApplicationBase(appName, argc, argv)
    {}

protected:
    Book::Result Setup() override;
    Book::Action ConsoleUiTest(Cmd::Switch &Arg);
    Signal<int,const char*, const std::string&> test{"Signal tests..."};

public:
    Book::Result Run() override;

    int Slot(const char* cchar, const std::string& stdstring);
};


Book::Action Application::ConsoleUiTest(Cmd::Switch &arg)
{
    using Ui::Console;
    AppBook::Message() << " Args:";
    for (auto const &A: arg.Arguments) {
        AppBook::Out() << A;
    }

    auto *Frame = new Ui::Frame((Util::Object*)nullptr, "The First Ui::Frame", Ui::WClass::TopLevel);
    auto *Label   = new Ui::Label(Frame,"That's Archlinux boyz!");
    auto *Icon    = new Ui::Icon(Frame);

    *Icon = Utf::Glyph::Debian;
    Icon->SetColors({Color::Red4, Color::Blue});
    Icon->SetPosition({3,2});
    Frame->SetGeometry(Dim{42,5});
    Label->SetPosition({5,2});
    Frame->SetPosition({2,2});
    Frame->SetCaption(Frame->Id());
    Frame->Show();
    Frame->Render({});
    Frame->Dispose();
    Console::GotoXY({1,12});
    Console::SetForegroundColor(Color::Reset);
    Ui::Element::PurgeGc();



    return Book::Action::Continue;
}


Book::Result Application::Run()
{
    if(!Setup()) return Book::Result::Failed;
    Args.Process();
    Signal<int, const char*, const std::string&>::Accumulator Accumulator{};
    test = Signal<int, const char*, const std::string&>("Signal Tests...", Accumulator);
    test.Connect(this, &Application::Slot);
    test("Hello","[std::string]::World!");
    for(auto V: Accumulator) Book::Debug() << " Accumulator :" << V;
    return Book::Result::Ok;
}


Book::Result Application::Setup()
{
    ApplicationBase::Setup();

    //...
    (Args << Cmd::Switch{"Ui::Test", "-w", "--UI::Console","Test Book::UI...",0 }).Connect(this, &Application::ConsoleUiTest);

    return CompileArguments();
}

int Application::Slot(const char * cchar, const std::string &stdstring)
{
    Book::Debug()  << cchar << ":" << stdstring;
    return 42;
}

} // namespace Book;

auto main(int argc, char** argv) -> int
{
    Book::Application App = Book::Application("AppBook:Window", argc,argv);
    return static_cast<int>(App.Run());
}
