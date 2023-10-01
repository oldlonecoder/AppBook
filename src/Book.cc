#include "logbook/Book.h"

#include <exception>
#include <filesystem>


using book::code;
using book::cat;

Book* Book::__Application_Book__{nullptr};
<<<<<<< HEAD

namespace fs = std::filesystem;
=======
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91


code Book::init(const std::string &book_name)
{
    if(Book::__Application_Book__) return code::exist;

    Book::__Application_Book__ = new Book(book_name);

    // --- Initialise env and filesystem / or the future sqlite database. Create it if not exists, open if exists.
    using fs = std::filesystem;
    stracc loc = fs::current_path();
    loc << id() <<".Book";
    if(! fs::exists(loc()) )
        fs::create_directory(loc());
    fs::current_path(loc());

    out_stream = &std::cout; // By default we set our out_stream to the address of the console's stdout. Subsequent creations of stack of elements will
                             // set it to the given output stream file / or not,  so out_stream always points to a valid output file.


    return code::accepted;
}


<<<<<<< HEAD




Book& Book::init(const std::string &book_name)
{
    if(Book::__Application_Book__)
        throw Book::exception("Book instance alredy created!");

    Book::__Application_Book__ = new Book(book_name);

    // --- Initialise env and filesystem / or the future sqlite database. Create it if not exists, open if exists.
    stracc loc = fs::current_path().c_str();
    loc << Book::__Application_Book__->id() <<".Book";
    if(! fs::exists(loc()) )
        fs::create_directory(loc());
    fs::current_path(loc());

    Book::__Application_Book__->out_stream = &std::cout; // By default we set our out_stream to the address of the console's stdout. Subsequent creations of stack of elements will
                             // set it to the given output stream file / or not,  so out_stream always points to a valid output file.


    return *Book::__Application_Book__;
}

Book &Book::Self()
{
    if(!Book::__Application_Book__)
        throw Book::exception("No Book instance yet! - Use Book& Book::init(const std::string &book_name) prior to use the `Book`");

    return *Book::__Application_Book__;
}


=======
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91


Book::Book()
{
    Book::__Application_Book__ = this;
}

<<<<<<< HEAD
Book::Book(const std::string &book_id):book::object(nullptr,book_id)
{

}


=======
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
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
<<<<<<< HEAD
book::code Book::open()
=======
book::rem::code Book::open()
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
{
    struct ent
    {
        bool is_set = false;
        fs::file_time_type htime;
        fs::directory_entry entry;
    }last_entry;

    fs::file_time_type htime;
    for(const auto& item : fs::directory_iterator(fs::current_path()))
    {
<<<<<<< HEAD
        if(item.is_directory())
        {
            auto const& tp = item.last_write_time();
            if(htime < tp)
            {
                last_entry.is_set = true;
                last_entry.entry = item;
=======
        if(entry.is_directory())
        {
            auto const& tp = entry.last_write_time();
            if(htime < tp)
            {
                last_entry.is_set = true;
                last_entry.entry = entry;
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
                last_entry.htime = tp;
            }
        }
    }
    if(!last_entry.is_set) return code::rejected;
<<<<<<< HEAD

    starting_path = fs::current_path().c_str();
    //...
    return code::success;
}

book::code Book::close()
=======
    //...

    return code::success;
}

book::rem::code Book::close()
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
{
    return book::code::notimplemented;
}


<<<<<<< HEAD
=======
Book::Book(const std::string &book_id):book::object(nullptr,book_id){}



>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
std::string Book::section::stack::get_filename()
{
    if(!Book::__Application_Book__)
        throw Book::exception("Book set!! ");
<<<<<<< HEAD

    return id() + (Book::__Application_Book__->_format ==  chattr::format::html ? ".html" : ".log");
}


book::code Book::section::stack::open()
{
    return code::notimplemented;
}

=======

    return id() + (Book::__Application_Book__->_format == attr::format::html ? ".html" : ".log");
}


book::code Book::section::stack::open()
{
    return code::notimplemented;
}
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
