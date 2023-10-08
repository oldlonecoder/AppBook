#include <logbook/Book.h>
#include <filesystem>




namespace fs = std::filesystem;




/*!
 * \brief Book::section::open open/select or create the location of the section identified by the section's name/id.
 * \return reference to the section instance.
 * \author &copy; 2023, oldlonecoder (sergre.lussier@oldlonecoder.club).
 */
Book::section::section(object *par, const std::string &section_id):book::object(par, section_id)
{

}

Book::section::~section()
{
    for(auto* b : blocs) delete b;
    blocs.clear();
}

book::code Book::section::close()
{
    for(auto* b : blocs)
    {
        b->close();
        delete b;
    }
    blocs.clear();
    return book::code::success;
}


Book::section &Book::section::open()
{
    // Get the Book (root) location.
    std::cout << __PRETTY_FUNCTION__ << ": location:" << location << ":\n";
    if(!fs::exists(location))
    {
        stracc err;
        err <<  "Section location '" << color::Yellow << location.c_str() << color::Reset << "' does not exist.";
        throw Book::exception(err().c_str());
    }
    return *this;
}



/*!
 * \brief Book::section::create_stack  Creates new bloc/stack/file of events & elements.
 * \param stack_id unique identifier relative to the parent section.
 * \return ref to the created instance
 */

Book::section::bloc_stack &Book::section::create_stack(const std::string &stack_id)
{

    blocs.push_back(new Book::section::bloc_stack(this, stack_id));
    Book::section::bloc_stack& bs = *blocs.back();

    if(bs.open() == book::code::success)
        std::cout << " bloc stack identified by [file] '" <<bs.id() << "' has been successfuly created.\n";

    return *blocs.back();
}

Book::section::bloc_stack &Book::section::operator[](const std::string &bloc_id)
{
    for(auto* blk : blocs)
    {
        if( blk->id() == bloc_id )
        {
            Book::Self().current_stream = blk;
            Book::Self().out_stream = &blk->output_file;
            std::cout << " bloc stack " << bloc_id << " is now set as current stream at the Book scope.\n";
            return *blk;
        }
    }
    stracc e;
    e << "bloc stack identified by '" << color::Yellow << bloc_id << color::Reset << "' not found.";
    throw Book::exception(e());
}
