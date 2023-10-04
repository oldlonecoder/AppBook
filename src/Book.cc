#include "logbook/Book.h"

#include <exception>
//#include <filesystem>


using book::code;
using book::cat;

Book* Book::__Application_Book__{nullptr};

namespace fs = std::filesystem;






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
    Book::__Application_Book__ = this;
}

Book::Book(const std::string &book_id):book::object(nullptr,book_id)
{

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

    for(const auto& item : fs::directory_iterator(fs::current_path()))
    {
        if(item.is_directory())
        {
            auto const& tp = item.last_write_time();
            if(htime < tp)
            {
                last_entry.is_set = true;
                last_entry.entry = item;
                last_entry.htime = tp;
            }
        }
    }
    if(!last_entry.is_set) return code::rejected;

    starting_path = fs::current_path().c_str();
    //...
    return code::success;
}

book::code Book::close()
{
    return book::code::notimplemented;
}

Book::section &Book::create_section(const std::string &section_id)
{
    sections.push_back(new Book::section(this, section_id));

    return *sections.back();
}



chattr::format Book::format()
{
    if(!Book::__Application_Book__)
        return chattr::format::ansi256;
    return Book::__Application_Book__->_format;
}


std::string Book::section::bloc_stack::get_filename()
{
    if(!Book::__Application_Book__)
        throw Book::exception("Attempt to use unitialized Book!");

    return id() + (Book::__Application_Book__->_format ==  chattr::format::html ? ".html" : ".log");
}

Book::section::bloc_stack::bloc_stack(object *parent_obj, const std::string &atitle): object(parent_obj, atitle)
{

}

Book::section::bloc_stack::~bloc_stack()
{
    content.clear();
}


book::code Book::section::bloc_stack::open()
{
    return code::notimplemented;
}

code Book::section::bloc_stack::commit()
{
    return book::code::notimplemented;
}


Book::section &Book::section::open()
{
    fs::path p = Book::Self().location; // Provoke exception throw if not set.

}
