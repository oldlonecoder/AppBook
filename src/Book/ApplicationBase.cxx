//
// Created by oldlonecoder on 4/1/24.
//

#include "AppBook/Book/ApplicationBase.h"
#include <csignal>


#define src_location std::source_location::current()
#define out_fun     std::cout << src_location.function_name() << " :" <<



void sig_int( int s )
{
    //rem::push_interrupted() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    //std::cerr << "\n---------------------------------------\n sig interrupt caught - flushing Book contents.\n ";
    AppBook::Close();
    exit(3);
}

void sig_fault( int s)
{
    //rem::push_segfault() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    //std::cerr << " sigfault caught...\n";
    Book::Segfault()  << " ...";
    AppBook::Close();
    exit(127);
}

void sig_abort( int s)
{

    //std::cerr << s << '\n';
    AppBook::Close(); //< Safe close - Only closes once - Can be called multiple times
    exit(127);
}


namespace Book
{

ApplicationBase* _APP{nullptr};

void ApplicationBase::InstallSignals()
{
    std::signal(SIGINT, sig_int);
    std::signal(SIGSEGV, sig_fault);
    std::signal(SIGABRT, sig_abort);
}

ApplicationBase::ApplicationBase()
{
    if(_APP)
    {
        std::cerr << Color::Ansi(Color::Red4) << " Error: " << Color::Ansi(Color::Reset) << "Cannot re-instance ApplicationBase class." << std::endl;
        exit(1);
    }
    _APP = this;
}


ApplicationBase::~ApplicationBase()
{
    mArgs.clear();
    auto R = AppBook::End();//< Safe End() : Alias to Close() - Only closes once - Can be called multiple times
    //auto [I,P] = Book::Enums::CodeAttributes(R);
    //StrAcc Str;
    //Str << Color::White << '[' << Color::Chartreuse6 << I << ' ' << P << Book::Enums::CodeText(R) << Color::Reset << ']';

    //out_fun " Return code from AppBook::End(): " << Str() << std::endl;
}

//Result ApplicationBase::Run()
//{
//    throw AppBook::Exception()[AppBook::Fatal() << "Cannot be invoked from this base ApplicationBase class!"];
//
//    return {};
//}

ApplicationBase::ApplicationBase(const std::string &AppName, int argc, char** argv) : Object(nullptr, AppName)
{
    _APP = this;
    //out_fun " Begin AppBook: " << AppName << ":\n";
    (void)AppBook::Begin(AppName);
    //AppBook::Debug() << "Book '" << AppName << "' created...";

    if(argc)
    {
        for(int a=1; a<argc; a++) mArgs.emplace_back(argv[a]);
    }

}



Book::Result ApplicationBase::Setup()
{
    // Systématique :
    Book::Select()["BookApi"]["Journal"];
    Book::Debug() << "\" Install signals:";
    ApplicationBase::InstallSignals();

    Book::Debug() << " ApplicationBase starting...";
    Con.GetGeometry();

    return Result::Success;
}

ApplicationBase& ApplicationBase::Instance()
{
    if(!_APP)
        throw AppBook::Exception() [ Book::Except() << " Reference to nullptr!" ];

    return *_APP;
}

ConIO::Console &ApplicationBase::Console()
{
    return Con;
}

Book::Result ApplicationBase::ProcessArguments()
{
    return Args.Input(mArgs);
}


}
