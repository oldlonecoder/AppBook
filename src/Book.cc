#include "logbook/Book.h"
#include <chrtools/textattr.h>
#include <exception>


//#include <filesystem>


using book::code;
using book::cat;


Book* Book::__Application_Book__{nullptr};

namespace fs = std::filesystem;



std::filesystem::path Book::location;


Book& Book::init(const std::string &book_name)
{
    if(Book::__Application_Book__)
        throw Book::exception("Book instance alredy created!");

    Book::__Application_Book__ = new Book(book_name);

    // --- Initialise env and filesystem / or the future sqlite database. Create it if not exists, open if exists.
    stracc loc = fs::current_path().c_str();
    loc << '/' << Book::__Application_Book__->id() <<".Book";
    if(! fs::exists(loc()) )
        fs::create_directory(loc());
    fs::current_path(loc());

    // Set the "root-" location of this application book; [ until the switch to sqlite ].
    Book::__Application_Book__->location = fs::current_path();

    // By default we set our out_stream to the address of the console's stdout. Subsequent creations of stack of elements will
    // set it to the given output stream file / or not,  so out_stream always points to a valid output file :
    Book::__Application_Book__->out_stream = &std::cout;

    // Return the ref to the (singleton) instance of the Book  :
    return *Book::__Application_Book__;
}

Book &Book::Self()
{
    if(!Book::__Application_Book__)
        throw Book::exception("No Book instance yet! - Use `Book& Book::init(const std::string &book_name)` prior to use the `Book`");

    return *Book::__Application_Book__;
}


Book::Book()
{
    if(Book::__Application_Book__)
        throw Book::exception(" Cannot re-instanciate a new Book!");
}

Book::Book(const std::string &book_id):book::object(nullptr,book_id)
{
    if(Book::__Application_Book__)
        throw Book::exception(" Cannot re-instanciate a new Book!");
}


Book::~Book()
{
    Book::__Application_Book__ = nullptr;
    //... Sections Chaining clear...
    //...
    for(auto* s: sections) delete s;

}


/*!
 * \brief Book::open  If the directory and subdirs already exist, then the Book select the last section created.
 * \return code::accepted
 * \author &copy; 2023, oldlonecoder ( serge.lussier@oldlonecoder.club )
 *
 * \note At the call of this method, the filesystem must already be positionned into the "$id().Book" subdirectory!
 */
book::code Book::open()
{
    struct ent
    {
        bool is_set = false;
        fs::file_time_type htime;
        fs::directory_entry entry;
    }last_entry;

    fs::file_time_type htime;
    auto Path = fs::current_path();

    std::cout << "current path: " << Path.c_str() << ":\n";

    for(const auto& item : fs::directory_iterator(location))
    {
        if(item.is_directory())
        {
            stracc dir;
            auto const& tp = item.last_write_time();
            dir << " subdir : '" << color::Yellow << item.path().c_str() << color::Reset << "': timestamp " << std::chrono::duration(tp.time_since_epoch())  << "\n";
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
        return code::rejected;
    }

    starting_path = fs::current_path().c_str();
    std::cout << "last entry:" << last_entry.entry.path().c_str() << "\n";
    //...
    return code::success;
}

book::code Book::close()
{
    if(!Book::__Application_Book__) return book::code::rejected;

    for(auto* s: Book::__Application_Book__->sections)
    {
        s->close();
        delete s;
    }
    Book::__Application_Book__->sections.clear();
    delete Book::__Application_Book__;

    return book::code::success;
}

Book::section &Book::operator[](std::string_view section_id)
{
    for(auto* s : sections) if( s->id() == section_id ) return *s;
    stracc e;
    e << "section identified by '" << color::Yellow << section_id << color::Reset << "' not found.";
    throw Book::exception(e());
}



/*!
 * \brief Book::create_section
 * \param section_id
 * \return ref to the new section instance.
 *
 * \author &copy; oldlonecoder ( serge.lussier@oldlonecoder.club )
 */
Book::section &Book::create_section(const std::string &section_id)
{
    fs::path check_location;
    check_location = location;
    check_location += '/';
    check_location += section_id;

    std::cout << " checking '" << check_location << ":\n";


    // -- Check instance os section identified by section_id first:
    auto sit = sections.begin();
    for(;sit != sections.end(); sit++) if((*sit)->id() == section_id) break;
    if(sit != sections.end()) return *(*sit);
    //-------------------------------------------------------------------------

    // -- Check if the section already exists in the filesytem:
    std::cout << " creating section identified by  '" << id() << ":\n";
    sections.push_back(new Book::section(this, section_id));
    Book::section& s = *sections.back();
    s.location = check_location;
    if(fs::exists(check_location))
    {
        stracc err;
        err << "Section location '" << color::Yellow << check_location.c_str() << color::Reset << "' already exists... Openning it.";
        std::cout << err() << "\n";
        return s;
    }
    //------------------------------------------------------------------------------------

    // Then create the section location in filesystem :
    std::cout << " creating '" <<  id() << "' location:\n";
    fs::create_directory(s.location);
    //------------------------------------------------------------------------------------
    std::cout << __FUNCTION__ << " done.\n";

    return s;
}



chattr::format Book::format()
{
    if(!Book::__Application_Book__)
        return chattr::format::ansi256;
    return Book::__Application_Book__->_format;
}



// ---------------------- inputs ---------------------------------------------------------------
#define CHECK_BOOK \
if(!Book::__Application_Book__  || !Book::__Application_Book__->current_stream || !Book::__Application_Book__->out_stream) \
    throw Book::exception("Using unset or uninitialized Book components.");

Book::section::bloc_stack::element& Book::error(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->error(std::move(src)); }
Book::section::bloc_stack::element& Book::out(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->out(std::move(src)); }
Book::section::bloc_stack::element& Book::warning(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->warning(std::move(src)); }
Book::section::bloc_stack::element& Book::fatal(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->fatal(std::move(src)); }
Book::section::bloc_stack::element& Book::except(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->except(std::move(src)); }
Book::section::bloc_stack::element& Book::message(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->message(std::move(src)); }
Book::section::bloc_stack::element& Book::debug(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->debug(std::move(src)); }
Book::section::bloc_stack::element& Book::info(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->info(std::move(src)); }
Book::section::bloc_stack::element& Book::comment(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->comment(std::move(src)); }
Book::section::bloc_stack::element& Book::syntax(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->syntax(std::move(src)); }
Book::section::bloc_stack::element& Book::status(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->status(std::move(src)); }
Book::section::bloc_stack::element& Book::test(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->test(std::move(src)); }
Book::section::bloc_stack::element& Book::interrupted(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->interrupted(std::move(src)); }
Book::section::bloc_stack::element& Book::aborted(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->aborted(std::move(src)); }
Book::section::bloc_stack::element& Book::segfault(book::source_location&& src) { CHECK_BOOK return Book::__Application_Book__->current_stream->segfault(std::move(src)); }

book::code Book::commit() { CHECK_BOOK return Book::Self().current_stream->commit(); }

// -------------------------------------------------------------------------------------
