//
// Created by oldlonecoder on 24-04-10.
//

#include <AppBook/Book/ApplicationBase.h>

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
    Book::ConIO::CWindow Window(nullptr, "Test Window");
    Window.SetGeometry({45,9});
    Window.SetScreenPosition({3,3});
    auto Pen = Window.GetPencil(Rect(Point{1, 1}, Dim{Window.Width()-2, Window.Height()-2}));

    Pen.Position({3, 3});
    Pen.Clear(Color::Navy);
    Pen << Color::LightGreen << Utf::Glyph::Admin
        << Color::Yellow6 << "Hello "
        << Color::Pair(Color::Grey100, Color::Reset)
        << "World!!!";

    Window.DrawFrame();
    StrAcc Text;
    Window >> Text;
    Book::Debug() << " Utf::CWindow :";
    Book::Out() << Text;
    Window.Draw();
    return Book::Action::Leave;
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
    (Args << Cmd::Switch{"CWindowTest",    "-w", "--WindowConsole","Test Book::ConIO::CWindow on Console...",0 }).Connect(this, &Application::ConsoleWindowTest);

    return ProcessArguments();
}

} // namespace Book;

auto main(int argc, char** argv) -> int
{
    Book::Application App = Book::Application("AppBook:CWindow", argc,argv);
    return static_cast<int>(App.Run());
}
