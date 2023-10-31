#include "AppBook/Book/AppBook.h"
#include "AppBook/Book/StmlText.h"

#include <exception>


//#include <filesystem>


using Book::Enums::Code;
using Book::Enums::Class;



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


AppBook& AppBook::Init(const std::string &book_name)
{
    if(AppBook::Application_Book)
        throw AppBook::Exception("Book instance already created!");

    AppBook::Application_Book = new AppBook(book_name);

    // --- Initialise env and filesystem / or the future sqlite database. Create it if not exists, open if exists.
    Core::StrAcc loc = Fs::current_path().c_str();
    loc << '/' << AppBook::Application_Book->Id() << ".Book";
    if(! Fs::exists(loc()) )
        Fs::create_directory(loc());
    Fs::current_path(loc());

    // Set the "root-" Location of this application book; [ until the switch to sqlite ].
    AppBook::LocationPath = Fs::current_path();

    // By default we set our out_stream to the address of the console's stdout. Subsequent creations of stack of elements will
    // set it to the given output stream file / or not,  so out_stream always points to a valid output file :
    AppBook::Application_Book->out_stream = &std::cout;

    // Return the ref to the (singleton) instance of the Book  :
    return *AppBook::Application_Book;
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

AppBook::AppBook(const std::string &book_id): Core::Object(nullptr,book_id)
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
 * \return code::accepted
 * \author &copy; 2023, oldlonecoder ( serge.lussier@oldlonecoder.club )
 *
 * \note At the call of this method, the filesystem must already be positionned into the "$id().Book" subdirectory!
 */
Book::Enums::Code AppBook::Open()
{
    struct ent
    {
        bool is_set = false;
        Fs::file_time_type htime;
        Fs::directory_entry entry;
    }last_entry;

    Fs::file_time_type htime;
    auto Path = Fs::current_path();

    std::cout << "current path: " << Path.c_str() << ":\n";

    for(const auto& item : Fs::directory_iterator(AppBook::LocationPath))
    {
        if(item.is_directory())
        {
            Core::StrAcc dir;
            auto const& tp = item.last_write_time();
            dir << " subdir : '" << Core::Color::Yellow << item.path().c_str() << Core::Color::Reset << "': timestamp " << std::chrono::duration(tp.time_since_epoch())  << "\n";
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
        return Book::Enums::Code::Rejected;
    }

    starting_path = Fs::current_path().c_str();
    std::cout << "last entry:" << last_entry.entry.path().c_str() << "\n";
    //...
    return Book::Enums::Code::Success;
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
    Core::StrAcc e;
    e << "Section identified by '" << Core::Color::Yellow << section_id << Core::Color::Reset << "' not found.";
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

    std::cout << " checking '" << check_location << ":\n";


    // -- Check instance os Section identified by section_id first:
    auto sit = Sections.begin();
    for(;sit != Sections.end(); sit++) if((*sit)->Id() == section_id) break;
    if(sit != Sections.end()) return *(*sit);
    //-------------------------------------------------------------------------

    // -- Check if the Section already exists in the filesytem:
    std::cout << " creating Section identified by  '" << Id() << ":\n";
    Sections.push_back(new AppBook::Section(this, section_id));
    AppBook::Section& s = *Sections.back();
    s.Location = check_location;
    if(Fs::exists(check_location))
    {
        Core::StrAcc err;
        err << "Section Location '" << Core::Color::Yellow << check_location.c_str() << Core::Color::Reset << "' already exists... Openning it.";
        std::cout << err() << "\n";
        return s;
    }
    //------------------------------------------------------------------------------------

    // Then create the Section Location in filesystem :
    std::cout << " creating '" <<  Id() << "' Location:\n";
    Fs::create_directory(s.Location);
    //------------------------------------------------------------------------------------
    std::cout << __FUNCTION__ << " done.\n";

    return s;
}



Core::Color::Format AppBook::Format()
{
    if(!AppBook::Application_Book)
        return Core::Color::Format::ansi256;
    return AppBook::Application_Book->_Format;
}



// ---------------------- inputs ---------------------------------------------------------------
#define CHECK_BOOK \
if(!AppBook::Application_Book  || !AppBook::Application_Book->current_stream || !AppBook::Application_Book->out_stream) \
    throw AppBook::Exception("Using unset or uninitialized Book components.");

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

// -------------------------------------------------------------------------------------
