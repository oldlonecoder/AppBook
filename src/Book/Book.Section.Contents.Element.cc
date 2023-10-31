#include "AppBook/Book/AppBook.h"
#include "AppBook/Core/StrBreak.h"
#include <chrono>
//#include <format>


using namespace Book::Enums;
using  Book::Enums::Class;

using Core::Color;




AppBook::Section::Contents::Element::Element(Object* par, Book::Enums::Class Cls, std::source_location asrc):Core::Object(par,"q-anon element!")
{
    Src = asrc;
    this->Class = Cls;
    this->Code = Code::Rejected;
}


/*!
 * \brief AppBook::Section::Contents::Element::cc_header -- build the header contents of this element.
 *
 * \author &copy;2023, oldlonecoder (serge.lussier@oldlonecoder.club).
 */
void AppBook::Section::Contents::Element::CompileHeader()
{
    //guard.lock();
    if(Ec.icon){;} // ...

    auto [icon, p] = ClassAttributes(this->Class);
    Text= "";
    if(this->Class != Class::Output)
    {
        //Get the color pair from the application DB for the given rem::type:
        if(this->Class == Class::Syntax)
        {
            auto [ic,sp] = ClassAttributes(Class::Error);
            Text , Color::White , '[' , p ,  Utf::Glyph::Data[icon] , sp, Utf::Glyph::Data[ic], ClassText(this->Class), ' ', ClassText(Class::Error), "] ";
        }
        else
            Text , Color::White , '[' , p ,  Utf::Glyph::Data[icon] ,  ClassText(this->Class) , Color::White , "] ";
    }

    auto const& header_components = AppBook::ComponentsHeaders["defaults"];
    if(this->Class != Class::Output) // Bypass output category.
    {
        if(header_components.filename)
        {
            Core::StrBreak txt;
            Core::StrBreak::ConfigData data = { Src.file_name(),"/",Core::StrBreak::Discard};
            auto cnt = txt(data);
            ///@todo check cnt....

            Text , Color::White , '[';
            Text , Color::CadetBlue2 , Utf::Glyph::Folder;
            if(cnt>1)
                Text , ".../";
            Text , data.Words.back()() , Color::White , ']';
        }
        if(header_components.line)
        {
            Text , Color::White , '|' , Color::Lime , Utf::Glyph::Baseline , Src.line() , Color::White , "|";
        }
        if(header_components.funcname || header_components.funcsig)
        {
            Text , Color::SteelBlue1 , Utf::Glyph::Function , Src.function_name() , Color::White , ": ";
        }
    } // Bypass output category.

}



Code AppBook::Section::Contents::Element::Compile()
{
    //guard.unlock();
    CompileHeader();
    for(auto const& comp: InputComponents)
        Text << comp;

    InputComponents.clear();
    return Code::Ok;
}

Code AppBook::Section::Contents::Element::Commit()
{
    return Parent<AppBook::Section::Contents>()->Commit();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Error        (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Error, asrc);
    return MemoryStream.back();
}

AppBook::Section::Contents::Element& AppBook::Section::Contents::Out          (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Output, asrc);
    return MemoryStream.back();
}

AppBook::Section::Contents::Element& AppBook::Section::Contents::Warning      (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Warning, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Fatal        (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Fatal, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Except       (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Except, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Message      (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Message, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Debug        (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Debug, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Info         (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Info, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Comment      (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Comment, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Syntax       (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Syntax, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Status       (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Status, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Test         (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Test, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Interrupted  (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Interrupted, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Aborted      (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Aborted, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Segfault     (std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Segfault, asrc);
    return MemoryStream.back();
}






AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (Utf::Glyph::Type Gh)
{
    Text << Gh;
    InputComponents.push_back(Text());
    Text.Clear();
    return *this;
}



AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (Utf::AccentFR::Type accent_code)
{
    Text << accent_code;
    InputComponents.push_back(Text());
    Text.Clear();
    return *this;
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (const Core::StrAcc& txt)
{
    Text << txt;
    InputComponents.push_back(Text());
    Text.Clear(); return *this;
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (const std::string& txt)
{
    Text << txt;
    InputComponents.push_back(Text());
    Text.Clear();
    return *this;
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (const char* txt)
{
    Text << txt;
    InputComponents.push_back(Text());
    Text.Clear();
    return *this;
}

AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (char c)
{
    Text << c;
    InputComponents.push_back(Text());
    Text.Clear();
    return *this;
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (Book::Enums::Code c)
{
    Text << Book::Enums::CodeText(c);
    InputComponents.push_back(Text());
    Text.Clear();
    return *this;
}

AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (Book::Enums::Class c)
{
    Text << ClassText(c);
    InputComponents.push_back(Text());
    Text.Clear();
    return *this;

}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (Color::code c)
{
    Text << c;
    InputComponents.push_back(Text());
    Text.Clear();
    return *this;
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (const Core::Rect& dd)
{
    Text << dd;
    InputComponents.push_back(Text());
    Text.Clear();
    return *this;
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator <<(Book::Enums::Action tr)
{
    switch(tr)
    {
    case Book::Enums::Action::Begin:
        break;
    case Book::Enums::Action::Ci:
    case Book::Enums::Action::End:
        Compile();
        Parent<AppBook::Section::Contents>()->OutputFile << Text();
        break;
    case Book::Enums::Action::Enter:
    case Book::Enums::Action::Leave:
        break;
    }

    return *this;
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (AppBook::ElementComponents cfg)
{
    Ec = cfg;
    return *this;
}


AppBook::Section::Contents::Element& AppBook::Section::Contents::Element::operator << (Book::Enums::Fn tr)
{
    switch(tr)
    {
    case Book::Enums::Fn::Endl:
        switch(AppBook::Format()) ///@todo create Book::format(); directly instead.
        {
            case Color::Format::ansi256: Text << "\n"; break;
            case Color::Format::html:    Text << "<br />"; break;
        }
        break;
        case Book::Enums::Fn::Stamp:
    {
        /*
             * %D %F ou %d %m %Y
             * %R %T %H %M %S
             * %A {Monday} %a {mon}
             * %B {June}   %b {jun}
        */


        //std::chrono::zoned_time date{"America/Toronto", std::chrono::system_clock::now()};
        //const auto tp{std::chrono::system_clock::now()};
        //auto txt{Core::StrAcc::now("{:%H:%M:%S}", tp)};
        auto [ic,a] = FnAttributes(Book::Enums::Fn::Stamp);
        Text << a.Fg << Utf::Glyph::Data[ic] << Color::Reset << Core::StrAcc::Now("%T");
    }
    break;
    case Book::Enums::Fn::File:
        Text << Src.file_name();
        break;
    case Book::Enums::Fn::Weekday:
    {
        auto [ic,a] = FnAttributes(Book::Enums::Fn::Weekday);
        //auto today{std::chrono::system_clock::now()};
        Text << /*Utf::Glyph::Data[ic] <<*/ a.Fg << Core::StrAcc::Now("%A");
    }
        break;
    case Book::Enums::Fn::Day :
    {
        auto [ic,a] = FnAttributes(Book::Enums::Fn::Day);
        //auto today{std::chrono::system_clock::now()};
        Text << /*Utf::Glyph::Data[ic] <<*/ a.Fg << Core::StrAcc::Now("%d");
    }
        break;
    case Book::Enums::Fn::Month:
    {
        auto [ic,a] = FnAttributes(Book::Enums::Fn::Month);
        //auto today{std::chrono::system_clock::now()};
        Text << /*Utf::Glyph::Data[ic] <<*/ a.Fg << Core::StrAcc::Now("%m");
    }

    break;
    case Book::Enums::Fn::Monthname:
    {
        auto [ic,a] = FnAttributes(Book::Enums::Fn::Month);
        //auto today{std::chrono::system_clock::now()};
        Text << /*Utf::Glyph::Data[ic] <<*/ a.Fg << Core::StrAcc::Now("%B");
    }

    break;

    case Book::Enums::Fn::Year:
    {
        auto [ic,a] = FnAttributes(Book::Enums::Fn::Year);
        //auto today{std::chrono::system_clock::now()};
        Text << /*Utf::Glyph::Data[ic] <<*/ a.Fg << Core::StrAcc::Now("%Y");
    }
    break;
    case Book::Enums::Fn::Function:
        Text << Src.function_name();
        break;
    default: break;
    }
    InputComponents.push_back(Text());
    Text.Clear();
    return *this;
}





