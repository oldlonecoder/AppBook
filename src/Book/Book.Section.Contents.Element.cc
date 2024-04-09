#include "AppBook/Book/AppBook.h"
#include "AppBook/Util/StrBreak.h"
#include <chrono>
//#include <format>


using namespace Book::Enums;
using Book::Enums::Class;


AppBook::Section::Contents::Element::Element(Object *par, Book::Enums::Class Cls, std::source_location asrc)
    : Object(par, "q-anon element!")
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
    if (Ec.icon) { ; } // ...

    auto [icon, p] = ClassAttributes(this->Class);
    Text = "";
    if (this->Class != Class::Output) {
        //Get the color pair from the application DB for the given rem::type:
        if (this->Class == Class::Syntax) {
            auto [ic, sp] = ClassAttributes(Class::Error);
            Text, Color::White, '[', p, Utf::Glyph::Data[icon], sp, Utf::Glyph::Data[ic], ClassText(this->Class), ' ', ClassText(Class::Error), "] ";
        } else
            Text, Color::White, '[', p, Utf::Glyph::Data[icon], ClassText(this->Class), Color::White, "] ";
    }

    auto const &header_components = AppBook::ComponentsHeaders["defaults"];
    if (this->Class != Class::Output) // Bypass output category.
    {
        if (header_components.filename) {
            StrBreak txt;
            StrBreak::ConfigData data = {Src.file_name(), "/", StrBreak::Discard};
            auto cnt = txt(data);
            ///@todo check cnt....

            Text, Color::White, '[';
            Text, Color::CadetBlue2, Utf::Glyph::Folder;
            if (cnt > 1)
                Text, ".../";
            Text, data.Words.back()(), Color::White, ']';
        }
        if (header_components.line) {
            Text, Color::White, '|', Color::Lime, Utf::Glyph::Baseline, Src.line(), Color::White, "|";
        }
        if (header_components.funcname || header_components.funcsig) {
            Text, Color::SteelBlue1, Utf::Glyph::Function, Src.function_name(), Color::White, ": ";
        }
    } // Bypass output category.

}


Code AppBook::Section::Contents::Element::Compile()
{
    //guard.unlock();
    CompileHeader();
    for (auto const &comp: InputComponents)
        Text << comp;

    InputComponents.clear();
    return Code::Ok;
}

Code AppBook::Section::Contents::Element::Commit()
{
    return Parent<AppBook::Section::Contents>()->Commit();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Error(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Error, asrc);
    return MemoryStream.back();
}

AppBook::Section::Contents::Element &AppBook::Section::Contents::Out(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Output, asrc);
    return MemoryStream.back();
}

AppBook::Section::Contents::Element &AppBook::Section::Contents::Warning(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Warning, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Fatal(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Fatal, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Except(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Except, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Message(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Message, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Debug(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Debug, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Info(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Info, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Comment(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Comment, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Syntax(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Syntax, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Status(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Status, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Test(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Test, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Interrupted(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Interrupted, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Aborted(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Aborted, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Segfault(std::source_location asrc)
{
    MemoryStream.emplace_back(this, Class::Segfault, asrc);
    return MemoryStream.back();
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(Utf::Glyph::Type Gh)
{
    //Text << Gh;
    //InputComponents.push_back(Text());
    //Text.Clear();
    Input(Gh);
    return *this;
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(Utf::AccentFR::Type accent_code)
{
    Input(accent_code);
    return *this;
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(const StrAcc &txt)
{
    Input(txt);
    return *this;
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(const std::string &Txt)
{
    Input(Txt);
    return *this;
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(const char *Txt)
{

    Input(Txt);
    return *this;
}

AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(char C)
{
    Input(C);
    return *this;
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(Book::Enums::Code E)
{
    Input(Book::Enums::CodeText(E));
    return *this;
}

AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(Book::Enums::Class C)
{
    Input(ClassText(C));
    return *this;
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(Color::Code C)
{
    Input(C);
    return *this;
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(const Rect &R)
{
    Input(R);
    return *this;
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(Book::Enums::Action A)
{
    switch (A) {
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


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(AppBook::ElementComponents cfg)
{
    Ec = cfg;
    return *this;
}


AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(Book::Enums::Fn tr)
{
    switch (tr) {
        case Book::Enums::Fn::Endl:
            switch (AppBook::Format()) ///@todo create Book::format(); directly instead.
            {
                case Color::Format::ansi256:
                    Input("\n");
                    break;
                case Color::Format::html:
                    Input("<br />");
                    break;
            }
            break;
        case Book::Enums::Fn::Stamp: {
            /*
                 * %D %F ou %d %m %Y
                 * %R %T %H %M %S
                 * %A {Monday} %a {mon}
                 * %B {June}   %b {jun}
            */


            //std::chrono::zoned_time date{"America/Toronto", std::chrono::system_clock::now()};
            //const auto tp{std::chrono::system_clock::now()};
            //auto txt{StrAcc::now("{:%H:%M:%S}", tp)};
            auto [ic, a] = FnAttributes(Book::Enums::Fn::Stamp);
            StrAcc Acc;
            Acc << a.Fg << Utf::Glyph::Data[ic] << Color::Reset << StrAcc::Now("%T");
            Input(Acc);
            return *this;
        }

        case Book::Enums::Fn::File:
            Input(Src.file_name());
            return *this;
        case Book::Enums::Fn::Weekday: {
            auto [ic, a] = FnAttributes(Book::Enums::Fn::Weekday);
            //auto today{std::chrono::system_clock::now()};
            StrAcc Acc;
            Acc << /*Utf::Glyph::Data[ic] <<*/ a.Fg << StrAcc::Now("%A");
            Input(Acc);
            return *this;
        }

        case Book::Enums::Fn::Day : {
            auto [ic, a] = FnAttributes(Book::Enums::Fn::Day);
            //auto today{std::chrono::system_clock::now()};
            StrAcc Acc;
            Acc << /*Utf::Glyph::Data[ic] <<*/ a.Fg << StrAcc::Now("%d");
            Input(Acc);
            return *this;
        }

        case Book::Enums::Fn::Month: {
            auto [ic, a] = FnAttributes(Book::Enums::Fn::Month);
            //auto today{std::chrono::system_clock::now()};
            StrAcc Acc;
            Acc << /*Utf::Glyph::Data[ic] <<*/ a.Fg << StrAcc::Now("%m");
            Input(Acc);
            return *this;
        }
        case Book::Enums::Fn::Monthname: {
            auto [ic, a] = FnAttributes(Book::Enums::Fn::Month);
            //auto today{std::chrono::system_clock::now()};
            StrAcc Acc;
            Acc << /*Utf::Glyph::Data[ic] <<*/ a.Fg << StrAcc::Now("%B");
            Input(Acc);
            return *this;
        }
        case Book::Enums::Fn::Year: {
            auto [ic, a] = FnAttributes(Book::Enums::Fn::Year);
            //auto today{std::chrono::system_clock::now()};
            StrAcc Acc;
            Acc << /*Utf::Glyph::Data[ic] <<*/ a.Fg << StrAcc::Now("%Y");
            Input(Acc);
            return *this;
        }
        case Book::Enums::Fn::Function:
            Input(Src.function_name());
            return *this;
        default:
            break;
    }
    InputComponents.push_back(Text());
    Text.Clear();
    return *this;
}

AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(Point Pt)
{
    Input(Pt.operator std::string());
    return *this;
}

AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(Dim WH)
{
    Input(WH.operator std::string());
    return *this;
}

//AppBook::Section::Contents::Element &AppBook::Section::Contents::Element::operator<<(Rect R)
//{
//    Input(R.operator std::string());
//    return *this;
//}
//
