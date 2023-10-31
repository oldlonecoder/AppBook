#include <AppBook/Book/AppBook.h>
#include <filesystem>




namespace fs = std::filesystem;


using Core::Color;
using namespace Book::Enums;

/*!
 * \brief AppBook::Sectionopen open/select or create the location of the section identified by the section's name/id.
 * \return reference to the section instance.
 * \author &copy; 2023, oldlonecoder (sergre.lussier@oldlonecoder.club).
 */
AppBook::Section::Section(Object *par, const std::string &section_id):Core::Object(par, section_id)
{

}

AppBook::Section::~Section()
{
    for(auto* b : Blocs) delete b;
    Blocs.clear();
}

Book::Enums::Code AppBook::Section::Close()
{
    for(auto* b : Blocs)
    {
        b->Close();
        delete b;
    }
    Blocs.clear();
    return Book::Enums::Code::Success;
}


AppBook::Section &AppBook::Section::Open()
{
    // Get the Book (root) location.
    std::cout << __PRETTY_FUNCTION__ << ": location:" << Location << ":\n";
    if(!fs::exists(Location))
    {
        Core::StrAcc err;
        err <<  "Section location '" << Color::Yellow << Location.c_str() << Color::Reset << "' does not exist.";
        throw AppBook::Exception(err().c_str());
    }
    return *this;
}



/*!
 * \brief AppBook::Sectioncreate_stack  Creates new bloc/stack/file of events & elements.
 * \param stack_id unique identifier relative to the parent section.
 * \return ref to the created instance
 */

AppBook::Section::Contents &AppBook::Section::CreateSectionContents(const std::string &stack_id)
{

    Blocs.push_back(new AppBook::Section::Contents(this, stack_id));
    AppBook::Section::Contents& bs = *Blocs.back();

    if(bs.Open() == Book::Enums::Code::Success)
        std::cout << " bloc stack identified by [file] '" <<bs.Id() << "' has been successfuly created.\n";

    return *Blocs.back();
}

AppBook::Section::Contents &AppBook::Section::operator[](const std::string &bloc_id)
{
    for(auto* blk : Blocs)
    {
        if( blk->Id() == bloc_id )
        {
            AppBook::Self().current_stream = blk;
            AppBook::Self().out_stream = &blk->OutputFile;
            std::cout << " bloc stack " << bloc_id << " is now set as current stream at the Book scope.\n";
            return *blk;
        }
    }
    Core::StrAcc e;
    e << "bloc stack identified by '" << Color::Yellow << bloc_id << Color::Reset << "' not found.";
    throw AppBook::Exception(e());
}
