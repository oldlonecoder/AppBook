#include "AppBook/Book/AppBook.h"
#include "AppBook/Book/StmlText.h"

#include <exception>
#include <utility>


//#include <filesystem>


using Book::Enums::Code;
using Book::Enums::Class;

#define CHECK_BOOK \
if(!AppBook::Application_Book  || !AppBook::Application_Book->current_stream || !AppBook::Application_Book->out_stream) \
    throw AppBook::Exception("Using unset or uninitialized Book components.");


AppBook* AppBook::Application_Book{nullptr};

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
        Fs::create_directory(loc());
    Fs::current_path(loc());

    // Set the "root-" Location of this application book; [ until the switch to sqlite ].
    AppBook::LocationPath = Fs::current_path();

    // By default we set our out_stream to the address of the console's stdout. Subsequent creations of stack of elements will
    // set it to the given output stream file / or not,  so out_stream always points to a valid output file :
    AppBook::Application_Book->out_stream = &std::cout;

    // Result the ref to the (singleton) instance of the Book  :
    return Book::Result::Success;
}

AppBook &AppBook::Self()
{
    if(!AppBook::Application_Book)
        throw AppBook::Exception("No Book instance yet! - Use `Book& Book::init(const std::string &book_name)` prior to use the `Book`");

    return *AppBook::Application_Book;
}


AppBook::AppBook()
{
    if(AppBook::Application_Book)
        throw AppBook::Exception(" Cannot re-instanciate a new Book!");
}

AppBook::AppBook(const std::string &book_id): Object(nullptr, book_id)
{
    if(AppBook::Application_Book)
        throw AppBook::Exception(" Cannot re-instanciate a new Book!");
}


AppBook::~AppBook()
{
    AppBook::Application_Book = nullptr;
    //... Sections Chaining Clear...
    //...
    for(auto* s: Sections) delete s;

}


/*!
 * \brief Book::open  If the directory and subdirs already exist, then the Book select the last Section created.
 * \return Code::accepted
 * \author &copy; 2023, oldlonecoder ( serge.lussier@oldlonecoder.club )
 *
 * \note At the call of this method, the filesystem must already be positionned into the "$id().Book" subdirectory!
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
    auto R = AppBook::Init();
    if(R != Book::Result::Success)
        throw AppBook::Exception(" Open Book failed!");


    Fs::file_time_type htime;
    auto Path = Fs::current_path();
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    StrAcc loc = Path.string().c_str();
#else 
    std::cout << "current path: " << Path.c_str() << ":\n";
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
            dir << " subdir : '" << Color::Yellow << item.path().c_str() << Color::Reset << "': timestamp " << std::chrono::duration(tp.time_since_epoch()) << "\n";
#endif
            
            std::cout << dir();
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
        std::cout << "This book has no contents ( no subdirs. ) - reject open request. \n";
        return *AppBook::Application_Book;
        // throw "exception thrown from  AppBook::Open";
    }
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    AppBook::Self().starting_path = Fs::current_path().string().c_str();
    std::cout << "last entry:" << last_entry.entry.path().string().c_str() << "\n";
#else 
    AppBook::Self().starting_path = Fs::current_path().c_str();
    std::cout << "last entry:" << last_entry.entry.path().c_str() << "\n";
#endif
    
    
    //...
    // return *this;
    return *AppBook::Application_Book;
}

Book::Enums::Code AppBook::Close()
{
    if(!AppBook::Application_Book) return Book::Enums::Code::Rejected;

    for(auto* s: AppBook::Application_Book->Sections)
    {
        s->Close();
        delete s;
    }
    AppBook::Application_Book->Sections.clear();
    delete AppBook::Application_Book;

    return Book::Enums::Code::Success;
}

AppBook::Section &AppBook::operator[](std::string_view section_id)
{
    for(auto* s : Sections) if( s->Id() == section_id ) return *s;
    StrAcc e;
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
    if(sit !=  AppBook::Application_Book->Sections.end()) return *(*sit);
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

[[maybe_unused]] AppBook::Section::Contents::Element& AppBook::Fatal       (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Fatal(src); }
AppBook::Section::Contents::Element& AppBook::Except      (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Except(src); }
AppBook::Section::Contents::Element& AppBook::Message     (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Message(src); }
AppBook::Section::Contents::Element& AppBook::Debug       (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Debug(src); }
AppBook::Section::Contents::Element& AppBook::Info        (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Info(src); }

[[maybe_unused]] AppBook::Section::Contents::Element& AppBook::Comment     (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Comment(src); }
AppBook::Section::Contents::Element& AppBook::Syntax      (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Syntax(src); }

[[maybe_unused]] AppBook::Section::Contents::Element& AppBook::Status      (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Status(src); }

[[maybe_unused]] AppBook::Section::Contents::Element& AppBook::Test        (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Test(src); }
AppBook::Section::Contents::Element& AppBook::Interrupted (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Interrupted(src); }
AppBook::Section::Contents::Element& AppBook::Aborted     (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Aborted(src); }
AppBook::Section::Contents::Element& AppBook::Segfault    (std::source_location src) { CHECK_BOOK return AppBook::Application_Book->current_stream->Segfault(src); }

Book::Enums::Code AppBook::Commit() { CHECK_BOOK return AppBook::Self().current_stream->Commit(); }

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
