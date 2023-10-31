#include "AppBook/Book/AppBook.h"

using namespace Book::Enums;
using Core::Color;


namespace fs = std::filesystem;


std::string AppBook::Section::Contents::GetFilename()
{
    if(!AppBook::Application_Book)
        throw AppBook::Exception("Attempt to use unitialized Book!");

    return Id() + (AppBook::Application_Book->_Format == Color::Format::html ? ".html" : ".log");
}



AppBook::Section::Contents::Contents(Core::Object *parent_obj, const std::string &atitle): Core::Object(parent_obj, atitle)
{

}

AppBook::Section::Contents::~Contents()
{
    Close();
    MemoryStream.clear();
}


Code AppBook::Section::Contents::Open()
{

    if(OutputFile.is_open())
        return Code::Rejected;

    Location = Parent<AppBook::Section>()->Location;
    Location += '/';
    Core::StrAcc file;
    file << Location.c_str() << GetFilename();
    Filename = file(); // Keep the copy of the filename

    std::cout << __PRETTY_FUNCTION__ << " - checking for existing '"<< Filename << "' :\n";
    OutputFile.open(file(),std::ios_base::out);
    if(!OutputFile.is_open())
    {
        Core::StrAcc str = "bloc_stack : cannot open file '";
        str << file << "' for output!'";
        throw AppBook::Exception(str());
    }
    std::cout << " file '" << Color::Ansi(Color::Yellow) << Filename << Color::Ansi(Color::Reset) << "' is open.\n";
    return Code::Success;
}



Code AppBook::Section::Contents::Close()
{
    if(!OutputFile.is_open())
        return Code::Rejected;

    Commit();
    OutputFile.flush();
    OutputFile.close();
    std::cout << " file '" << Color::Ansi(Color::Yellow) << Filename << Color::Ansi(Color::Reset) << "' is closed.\n";
    return Code::Success;
}



/*!
 * \brief AppBook::Section::Contents::commit  - or flush and clear.
 * \return accepted or rejected;
 */
Code AppBook::Section::Contents::Commit()
{
    if(!OutputFile.is_open()) throw AppBook::Exception("AppBook::Section::Contents::commit() : no output file.");
    for(auto& elem: MemoryStream)
    {
        elem.Compile();
        OutputFile << elem.Text() << std::endl;
        elem.Text.Clear();
    }

    if(!MemoryStream.empty()) MemoryStream.clear(); // Don't call it if Empty...

    return Code::Accepted;
}

