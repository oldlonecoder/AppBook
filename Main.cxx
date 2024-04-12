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
    AppBook::Message() << " Args:";
    for (auto const &A: arg.Arguments) {
        AppBook::Out() << A;
    }

    auto *Element = new ConsoleUI::UiElement((Util::Object*)nullptr, "First UI Element", Ui::WClass::Frame);

    Element->SetGeometry(Dim{3,3});

    Element->GotoXY({1,1});
    Element->SetBgColor(Color::LightCyan3);
    Element->PutGlyph(Utf::Glyph::Admin);
    //...
    Element->Dispose();
    Book::ConsoleUI::Console::GotoXY({3,3});
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
