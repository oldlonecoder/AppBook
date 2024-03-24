#include "AppBook/Book/AppBook.h"
#include "AppBook/Book/StmlText.h"

#include <exception>
#include <utility>


//#include <filesystem>


using Book::Enums::Code;
using Book::Enums::Class;

#define src_location std::source_location::current()
#define out_fun     std::cout << src_location.function_name() << " :" <<

#define CHECK_BOOK \
if(!AppBook::Application_Book  || !AppBook::Application_Book->current_stream || !AppBook::Application_Book->out_stream) AppBook::ThrowOnNoStream();



AppBook* AppBook::Application_Book{nullptr};

std::stack<AppBook::Section::Contents*> AppBook::ContextStack{};





namespace Fs = std::filesystem;

/*
        uint16_t glyph     : 1; // Icon/Glyph on element components
        uint16_t Filename : 1; // Filename ...
        uint16_t line     : 1; // line number
        uint16_t column   : 1; // column number
        uint16_t funcname : 1; // function name ( short )
        uint16_t funcsig  : 1; // __PRETTY_FUNCTION__  ( full function sisgnature }
        uint16_t stamp    : 1; // time stamp ( full calendar details ( excepted second's floating precisions)
        uint16_t time     : 1; // Time of the day
        uint16_t day      : 1; // day of the month
        uint16_t month    : 1; // month of the year
        uint16_t year     : 1; // year...
 */
AppBook::ElementComponents::Dictionary AppBook::ComponentsHeaders = {
    {
        // [glyph | Filename | line | column | funcname | time]
        {"defaults", {1,1,1,1,1,0,0,1,0,0,0}},
        // [glyph  | time | day | month | year]
        {"section_header",{1,0,0,0,0,1,1,1,1}},

    }
};



std::filesystem::path AppBook::LocationPath;


Book::Result AppBook::Init()
{
    // --- Initialise env and filesystem / or the future sqlite database. Create it if not exists, open if exists.
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    StrAcc loc = Fs::current_path().string().c_str();
#else 
    StrAcc loc = Fs::current_path().c_str();
#endif
    loc << '/' << AppBook::Application_Book->Id() << ".Book";
    if(! Fs::exists(loc()) )
    {
        out_fun " --> Create '" << loc() << "' subdirectory:" << std::endl;
        Fs::create_directory(loc());
    }
    Fs::current_path(loc());

    // Set the "root-" Location of this application book; [ until the switch to sqlite ].
    AppBook::LocationPath = Fs::current_path();

    // By default we set our out_stream to the address of the console's stdout. Subsequent creations of stack of elements will
    // set it to the given output stream file / or not,  so out_stream always points to a valid output file :
    out_fun "Setting output stream to the std::cout address by default, will be replaced by subsequent section::content stream..." << std::endl;
    AppBook::Application_Book->out_stream = &std::cout;

    // Result the ref to the (singleton) instance of the Book  :
    out_fun ": AppBook::LocationPath is set and it is now in " << AppBook::LocationPath << std::endl;
    return Book::Result::Success;
}

AppBook &AppBook::Self()
{
    if(!AppBook::Application_Book)
        throw AppBook::Exception("No Book instance yet! - Use `AppBook& AppBook::Begin|Open(const std::string &BookName)` prior to use the `AppBook`");

    return *AppBook::Application_Book;
}


AppBook::AppBook()
{
    if(!AppBook::Application_Book)
        throw AppBook::Exception("Application Book must be instantiated through the static AppBook::Open('BookName')!");

}

AppBook::AppBook(const std::string &book_id): Object(nullptr, book_id)
{
    if(AppBook::Application_Book)
        throw AppBook::Exception(" Cannot re-instance a new Book!");
}


AppBook::~AppBook()
{
    //AppBook::Application_Book = nullptr;
    //... Sections Chaining Clear...
    //...

    for(auto* s: Sections) delete s;

}


/*!
 * \brief Book::open  If the directory and subdirs already exist, then the Book select the last Section created.
 * \return Code::accepted
 * \author &copy; 2023, oldlonecoder ( serge.lussier@oldlonecoder.club )
 *
 * \note We call this method to create or position to the Id().Book subdir.
 */
AppBook& AppBook::Open(const std::string& BookName)
{

    struct ent
    {
        bool is_set = false;
        Fs::file_time_type htime;
        Fs::directory_entry entry;
    }last_entry;

    if(AppBook::Application_Book)
        throw AppBook::Exception("Book instance already created!");

    AppBook::Application_Book = new AppBook(BookName);
    (void) AppBook::Init();
//    if(R != Book::Result::Success)
//        throw AppBook::Exception(" Open Book failed!");

    out_fun " Application Book " << AppBook::Instance().Id() << "  Created\n";

    Fs::file_time_type htime;
    auto Path = Fs::current_path();
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    StrAcc loc = Path.string().c_str();
#else 
    out_fun "current path: " << Path.c_str() << ":\n";
#endif
    

    for(const auto& item : Fs::directory_iterator(AppBook::LocationPath))
    {
        if(item.is_directory())
        {
            StrAcc dir;
            auto const& tp = item.last_write_time();
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            dir << " subdir : '" << Color::Yellow << item.path().string().c_str() << Color::Reset << "': timestamp " << std::chrono::duration(tp.time_since_epoch()) << "\n";
#else 
            dir << " subdir : '" << Color::Yellow << item.path().c_str() << Color::Reset << '\n';
#endif
            
            std::cout << std::source_location::current().function_name() <<  ":" << dir();
            if(!last_entry.is_set)
            {
                last_entry.is_set = true;
                last_entry.entry = item;
                last_entry.htime = tp;
            }
            else
            {
                if(tp > last_entry.htime)
                {
                    last_entry.is_set = true;
                    last_entry.entry = item;
                    last_entry.htime = tp;
                }
            }
        }
    }
    if(!last_entry.is_set)
    {
        out_fun src_location.line() << " :"  << "This book has no section(s) yet (subdir is empty) - leaving ( Goto: ApiLog section create). \n";


         //throw AppBook::Exception("exception thrown from  AppBook::Open : no entry");
    }
    else {
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        AppBook::Self().starting_path = Fs::current_path().string().c_str();
        std::cout << "last entry:" << last_entry.entry.path().string().c_str() << "\n";
#else
        AppBook::Self().starting_path = Fs::current_path().c_str();
        std::cout << "last entry:" << last_entry.entry.path().c_str() << "\n";
#endif
    }
    AppBook::SetupBookApiJournal();
    return *AppBook::Application_Book;
}


Book::Enums::Code AppBook::Close()
{
    if(!AppBook::Application_Book) return Book::Enums::Code::Rejected;

    if(AppBook::Application_Book->Sections.empty()) return Book::Result::Empty;
    for(auto* s: AppBook::Application_Book->Sections)
    {
        s->Close();
        delete s;
    }
    AppBook::Application_Book->Sections.clear();
    delete AppBook::Application_Book;
    AppBook::Application_Book = nullptr;

    return Book::Enums::Code::Success;
}

AppBook::Section &AppBook::operator[](std::string_view section_id)
{
    StrAcc e;

    if(!AppBook::Application_Book)
    {
        e << "Application Book not Opened! Instantiate through AppBook::Open('BookName')";
        throw AppBook::Exception(e());
    }

    for(auto* s : AppBook::Application_Book->Sections) if( s->Id() == section_id ) return *s;
    e << "Section identified by '" << Color::Yellow << section_id << Color::Reset << "' not found.";
    throw AppBook::Exception(e());
}



/*!
 * \brief Book::create_section
 * \param section_id
 * \return ref to the new Section instance.
 *
 * \author &copy; oldlonecoder ( serge.lussier@oldlonecoder.club )
 */
AppBook::Section &AppBook::CreateSection(const std::string &section_id)
{
    Fs::path check_location;
    check_location = AppBook::LocationPath;
    check_location += '/';
    check_location += section_id;

    std::cout << " checking '" << Color::Ansi(Color::Yellow) << check_location << Color::Ansi(Color::Reset) << "':\n";


    // -- Check instance os Section identified by section_id first:
    //CHECK_BOOK
    auto sit =  AppBook::Application_Book->Sections.begin();
    for(;sit !=  AppBook::Application_Book->Sections.end(); sit++) if((*sit)->Id() == section_id) break;
    if(sit !=  AppBook::Application_Book->Sections.end())
    {
        out_fun "Section '" << section_id << "' found and returning its instance.";
        return *(*sit);
    }
    //-------------------------------------------------------------------------

    // -- Check if the Section already exists in the filesytem:
    std::cout << " creating Section identified by  '" <<  AppBook::Application_Book->Id() << ":\n";
    AppBook::Application_Book->Sections.push_back(new AppBook::Section( AppBook::Application_Book, section_id));
    AppBook::Section& s = * AppBook::Application_Book->Sections.back();
    s.Location = check_location;
    if(Fs::exists(check_location))
    {
        StrAcc err;
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        err << "Section Location '" << Color::Yellow << check_location.string().c_str() << Color::Reset << "' already exists... Openning it.";
#else 
        err << "Section Location '" << Color::Yellow << check_location.c_str() << Color::Reset << "' already exists... Openning it.";
#endif
        
        std::cout << err() << "\n";
        return s;
    }
    //------------------------------------------------------------------------------------

    // Then create the Section Location in filesystem :
    std::cout << " creating '" <<   AppBook::Application_Book->Id() << "' Location:\n";
    Fs::create_directory(s.Location);
    //------------------------------------------------------------------------------------
    std::cout << __FUNCTION__ << " done.\n";

    return s;
}



Color::Format AppBook::Format()
{
    if(!AppBook::Application_Book)
        return Color::Format::ansi256;
    return AppBook::Application_Book->_Format;
}



// ---------------------- inputs ---------------------------------------------------------------

AppBook::Section::Contents::Element& AppBook::Out         (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Out(src); }
AppBook::Section::Contents::Element& AppBook::Warning     (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Warning(src); }
AppBook::Section::Contents::Element& AppBook::Error       (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Error(src); }
AppBook::Section::Contents::Element& AppBook::Fatal       (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Fatal(src); }
AppBook::Section::Contents::Element& AppBook::Except      (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Except(src); }
AppBook::Section::Contents::Element& AppBook::Message     (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Message(src); }
AppBook::Section::Contents::Element& AppBook::Debug       (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Debug(src); }
AppBook::Section::Contents::Element& AppBook::Info        (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Info(src); }
AppBook::Section::Contents::Element& AppBook::Comment     (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Comment(src); }
AppBook::Section::Contents::Element& AppBook::Syntax      (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Syntax(src); }
AppBook::Section::Contents::Element& AppBook::Status      (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Status(src); }
AppBook::Section::Contents::Element& AppBook::Test        (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Test(src); }
AppBook::Section::Contents::Element& AppBook::Interrupted (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Interrupted(src); }
AppBook::Section::Contents::Element& AppBook::Aborted     (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Aborted(src); }
AppBook::Section::Contents::Element& AppBook::Segfault    (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Segfault(src); }

Book::Enums::Code AppBook::Commit() { CHECK_BOOK return AppBook::Self().current_stream->Commit(); }

AppBook &AppBook::Instance()
{
    if(!AppBook::Application_Book)
    {
        StrAcc Str;
        Str << Color::Red4 << "Error: " << Color::White << " Application Book was not created! ";
        throw AppBook::Exception(Str());
    }
    return *AppBook::Application_Book;
}

AppBook &AppBook::Begin(const std::string &BookName)
{
    return AppBook::Open(BookName);
}

Book::Enums::Code AppBook::End()
{
    return AppBook::Close();
}

void AppBook::ThrowOnNoStream()
{

    StrAcc Msg;
    Msg <<  Color::Red4 << "Error: " << Color::White <<"Throwing AppBook::Exception while accessing one of AppBook output methods:\n";

    if(AppBook::Application_Book)
    {
        AppBook& Ins = AppBook::Instance();

        if(!Ins.current_stream)
            Msg << " --> Section::Contents was unset/or un-selected yet";
        if(!Ins.out_stream)
            Msg << "\n --> Output file stream of " << Ins.current_stream->Parent<AppBook::Section>()->Id() << " was not set yet.";
    }
    else
        Msg << " --> The Application Book was not yet created.\n";

    throw AppBook::Exception(Msg());
}

Book::Result AppBook::PushContext()
{
    CHECK_BOOK

    AppBook::ContextStack.push(AppBook::Application_Book->current_stream);
    return Code::Ok;
}

Book::Result AppBook::PopContext()
{
    CHECK_BOOK

    if(AppBook::Application_Book->Sections.empty())
        return Book::Result::Empty;

     AppBook::Application_Book->current_stream =  AppBook::ContextStack.top();
    return Code::Accepted;
}

Book::Result AppBook::SetupBookApiJournal()
{
    AppBook::CreateSection("BookApi").Open().CreateSectionContents("Journal");
    AppBook()["BookApi"]["Journal"];
    AppBook::Debug() << " Automatic implicit BookApi::Journal is setup." ;
    return Code::Success;
}


// -------------------------------------------------------------------------------------
AppBook::Exception AppBook::Exception::operator[](AppBook::Section::Contents::Element El)
{
    Element = std::move(El);
    return *this;
}

const char *AppBook::Exception::what() const noexcept
{
    if(E)
    {
        AppBook::Section::Contents::Element EE = Element;
        EE.Compile();
        return EE.Text().c_str();
    }
    return msg.c_str();
}


namespace Book
{
AppBook::Section::Contents::Element &Error      (std::source_location src){ return AppBook::Error      (src);}
AppBook::Section::Contents::Element &Out        (std::source_location src){ return AppBook::Out        (src);}
AppBook::Section::Contents::Element &Warning    (std::source_location src){ return AppBook::Warning    (src);}
AppBook::Section::Contents::Element &Fatal      (std::source_location src){ return AppBook::Fatal      (src);}
AppBook::Section::Contents::Element &Except     (std::source_location src){ return AppBook::Except     (src);}
AppBook::Section::Contents::Element &Message    (std::source_location src){ return AppBook::Message    (src);}
AppBook::Section::Contents::Element &Debug      (std::source_location src){ return AppBook::Debug      (src);}
AppBook::Section::Contents::Element &Info       (std::source_location src){ return AppBook::Info       (src);}
AppBook::Section::Contents::Element &Comment    (std::source_location src){ return AppBook::Comment    (src);}
AppBook::Section::Contents::Element &Syntax     (std::source_location src){ return AppBook::Syntax     (src);}
AppBook::Section::Contents::Element &Status     (std::source_location src){ return AppBook::Status     (src);}
AppBook::Section::Contents::Element &Test       (std::source_location src){ return AppBook::Test       (src);}
AppBook::Section::Contents::Element &Interrupted(std::source_location src){ return AppBook::Interrupted(src);}
AppBook::Section::Contents::Element &Aborted    (std::source_location src){ return AppBook::Aborted    (src);}
AppBook::Section::Contents::Element &Segfault   (std::source_location src){ return AppBook::Segfault   (src);}

AppBook& Select()
{
    return AppBook::Self();
}

}