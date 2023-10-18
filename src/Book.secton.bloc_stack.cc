#include <logbook/Book.h>

using namespace book;
namespace fs = std::filesystem;

std::string Book::section::bloc_stack::get_filename()
{
    if(!Book::Application_Book)
        throw Book::exception("Attempt to use unitialized Book!");

    return id() + (Book::Application_Book->_format == chattr::format::html ? ".html" : ".log");
}



Book::section::bloc_stack::bloc_stack(object *parent_obj, const std::string &atitle): object(parent_obj, atitle)
{

}

Book::section::bloc_stack::~bloc_stack()
{
    close();
    content.clear();
}


code Book::section::bloc_stack::open()
{

    if(output_file.is_open())
        return book::code::rejected;

    location = parent<Book::section>()->location;
    location += '/';
    stracc file;
    file << location.c_str() << get_filename();
    filename = file(); // Keep the copy of the filename

    std::cout << __PRETTY_FUNCTION__ << " - checking for existsing '"<< filename << "' :\n";
    output_file.open(file(),std::ios_base::out);
    if(!output_file.is_open())
    {
        stracc str = "bloc_stack : cannot open file '";
        str << file << "' for output!'";
        throw Book::exception(str());
    }
    std::cout << " file '" << chattr::ansi(color::Yellow) << filename << chattr::ansi(color::Reset) << "' is open.\n";
    return code::success;
}



code Book::section::bloc_stack::close()
{
    if(!output_file.is_open())
        return book::code::rejected;

    commit();
    output_file.flush();
    output_file.close();
    std::cout << " file '" << chattr::ansi(color::Yellow) << filename << chattr::ansi(color::Reset) << "' is closed.\n";
    return book::code::success;
}



/*!
 * \brief Book::section::bloc_stack::commit  - or flush and clear.
 * \return accepted or rejected;
 */
code Book::section::bloc_stack::commit()
{
    if(!output_file.is_open()) throw Book::exception("Book::section::bloc_stack::commit() : no output file.");
    for(auto& elem: content)
    {
        elem.cc();
        output_file << elem.text() << std::endl;
        elem.text.clear();
    }

    if(!content.empty()) content.clear(); // Don't call it if empty...

    return code::accepted;
}

