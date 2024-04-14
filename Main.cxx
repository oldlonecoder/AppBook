//
// Created by oldlonecoder on 24-04-10.
//

#include <AppBook/Book/ApplicationBase.h>
#include <AppBook/ConIO/Widget/Label.h>
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
    using Ui::Console;
    AppBook::Message() << " Args:";
    for (auto const &A: arg.Arguments) {
        AppBook::Out() << A;
    }

    auto *Element = new Ui::UiElement((Util::Object*)nullptr, "First UI Element", Ui::WClass::Frame);
    auto *Label   = new Ui::Label(Element,"It's Debian Sir.");
    Element->SetColors({Color::Blue,Color::Yellow});

    Label->SetColors({Color::Blue,Color::Yellow});
    Label->SetPosition({1,1});
    Label->SetLeftGlyph(Utf::Glyph::Debian, {Color::Blue,Color::Maroon});

    Element->SetGeometry(Dim{22,3});

    Element->GotoXY({1,1});
    Element->SetColors({Color::Blue,Color::Yellow});

    Element->Show();
    Element->Render();
    //...
    Element->Dispose();
    Console::GotoXY({1,Console::Cursor.Y+2});
    Console::SetForegroundColor(Color::Maroon);
    Console::SetBackgroundColor(Color::Reset);

//    Console::Write(Utf::Glyph::Data[Utf::Glyph::Debian],true);
    //...
    Ui::UiElement::PurgeGc();
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
    (Args << Cmd::Switch{"ConIO::Test",    "-w", "--WindowConsole","Test Book::ConIO::Window on ConIO...",0 }).Connect(this, &Application::ConsoleWindowTest);

    return ProcessArguments();
}

} // namespace Book;

auto main(int argc, char** argv) -> int
{
    Book::Application App = Book::Application("AppBook:Window", argc,argv);
    return static_cast<int>(App.Run());
}
