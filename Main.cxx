//
// Created by oldlonecoder on 24-04-10.
//

#include <AppBook/Book/ApplicationBase.h>
#include <AppBook/ConsoleUI/UiElement.h>
namespace Book
{


class Application : public Book::ApplicationBase {

public:

    Application(const std::string &appName, int argc, char **argv) : ApplicationBase(appName, argc, argv)
    {}

protected:
    Book::Result Setup() override;
    Book::Action ConsoleWindowTest(Cmd::Switch &Arg);
public:
    Book::Result Run() override;


};


Book::Action Application::ConsoleWindowTest(Cmd::Switch &arg)
{
    using ConsoleUI::Console;
    AppBook::Message() << " Args:";
    for (auto const &A: arg.Arguments) {
        AppBook::Out() << A;
    }

    auto *Element = new ConsoleUI::UiElement((Util::Object*)nullptr, "First UI Element", Ui::WClass::Frame);

    Element->SetGeometry(Dim{20,3});

    Element->GotoXY({1,1});
    Element->SetFgColor(Color::Yellow);
    Element->SetBgColor(Color::Blue);
    Element->Clear();
    ConsoleUI::Console::RenderElement(Element,{});
    //...
    Element->Dispose();
    Console::GotoXY({3,2});
    Console::SetForegroundColor(Color::Maroon);
    Console::SetBackgroundColor(Color::Blue);
    Console::Write(Utf::Glyph::Data[Utf::Glyph::Debian],true);
    Console::SetForegroundColor(Color::Reset);
    Book::ConsoleUI::Console::GotoXY({3,5});
    std::cout << "Hey!!!!!!\n";


    //...
    ConsoleUI::UiElement::PurgeGc();
    return Book::Action::Continue;
}


Book::Result Application::Run()
{
    if(!Setup()) return Book::Result::Failed;
    Args.Process();
    return Book::Result::Ok;
}

Book::Result Application::Setup()
{
    ApplicationBase::Setup();


    //...
    (Args << Cmd::Switch{"CWindowTest",    "-w", "--WindowConsole","Test Book::ConsoleUI::Window on ConsoleUI...",0 }).Connect(this, &Application::ConsoleWindowTest);

    return ProcessArguments();
}

} // namespace Book;

auto main(int argc, char** argv) -> int
{
    Book::Application App = Book::Application("AppBook:Window", argc,argv);
    return static_cast<int>(App.Run());
}
