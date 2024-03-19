#include "AppBook/Book/AppBook.h"

using namespace Book::Enums;



namespace fs = std::filesystem;


std::string AppBook::Section::Contents::GetFilename()
{
    if(!AppBook::Application_Book)
        throw AppBook::Exception("Attempt to use unitialized Book!");

    return Id() + (AppBook::Application_Book->_Format == Color::Format::html ? ".html" : ".log");
}



AppBook::Section::Contents::Contents(Util::Object *parent_obj, const std::string &atitle): Util::Object(parent_obj, atitle)
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
    StrAcc file;
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    file << Location.string().c_str() << GetFilename();
#else 
    file << Location.c_str() << GetFilename();
#endif

    Filename = file(); // Keep the copy of the filename

    std::cout << __PRETTY_FUNCTION__ << " - checking for existing '"<< Filename << "' :\n";
    OutputFile.open(file(),std::ios_base::out);
    if(!OutputFile.is_open())
    {
        StrAcc str = "bloc_stack : cannot open file '";
        str << file << "' for output!'";
        throw AppBook::Exception(str());
    }
    std::cout << " file '" << Color::Ansi(Color::Yellow) << Filename << Color::Ansi(Color::Reset) << "' is open.\n";
    StrAcc Msg;
    Msg << Color::DeepSkyBlue7 << Parent<AppBook::Section>()->Id() << Color::White << "::" << Color::Yellow <<  Id() << Color::White << " logs\n";
    OutputFile << Msg();
    OutputFile << "--------------------------------------------------\n";

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

