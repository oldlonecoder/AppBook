#include "logbook/Book.h"
#include <chrtools/strbrk.h>
#include <chrono>
//#include <format>


Book::section::bloc_stack::element::element(object* par, book::cat category, std::source_location asrc):book::object(par,"q-anon element!")
{
    src = asrc;
    cat = category;
    code = book::code::rejected;
}


/*!
 * \brief Book::section::bloc_stack::element::cc_header -- build the header contents of this element.
 *
 * \author &copy;2023, oldlonecoder (serge.lussier@oldlonecoder.club).
 */
void Book::section::bloc_stack::element::cc_header()
{
    //guard.lock();
    if(ec.icon){;} // ...

    auto [icon, p] = book::category_attributes(cat);
    text= "";
    if(cat != book::cat::output)
    {
        //Get the color pair from the application DB for the given rem::type:
        if(cat == book::cat::syntax)
        {
            auto [ic,sp] = book::category_attributes(book::cat::error);
            text , color::White , '[' , p ,  Icon::Data[icon] , sp, Icon::Data[ic], book::category_text(cat), ' ', book::category_text(book::cat::error), "] ";
        }
        else
            text , color::White , '[' , p ,  Icon::Data[icon] ,  book::category_text(cat), color::White , "] ";
    }

    auto const& header_components = Book::header_components_db["defaults"];
    if(cat != book::cat::output) // Bypass output category.
    {
        if(header_components.filename)
        {
            strbrk txt;
            strbrk::config_data data = { src.file_name(),"/",strbrk::discard};
            auto cnt = txt(data);
            ///@todo check cnt....

            text , color::White , '[';
            text , color::CadetBlue2 , Icon::Folder;
            if(cnt>1)
                text , ".../";
            text , data.words.back()() , color::White , ']';
        }
        if(header_components.line)
        {
            text , color::White , '|' , color::Lime , Icon::Baseline , src.line() , color::White , "|";
        }
        if(header_components.funcname || header_components.funcsig)
        {
            text , color::SteelBlue1 , Icon::Function , src.function_name() , color::White , ": ";
        }
    } // Bypass output category.

}



book::code Book::section::bloc_stack::element::cc()
{
    //guard.unlock();
    cc_header();
    for(auto const& comp: input_components)
        text << comp;

    input_components.clear();
    return book::code::ok;
}

book::code Book::section::bloc_stack::element::commit()
{
    return parent<Book::section::bloc_stack>()->commit();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::error        (std::source_location asrc)
{
    content.push_back({this, book::cat::error, asrc});
    return content.back();
}

Book::section::bloc_stack::element& Book::section::bloc_stack::out          (std::source_location asrc)
{
    content.push_back({this, book::cat::output, asrc});
    return content.back();
}

Book::section::bloc_stack::element& Book::section::bloc_stack::warning      (std::source_location asrc)
{
    content.push_back({this, book::cat::warning, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::fatal        (std::source_location asrc)
{
    content.push_back({this, book::cat::fatal, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::except       (std::source_location asrc)
{
    content.push_back({this, book::cat::except, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::message      (std::source_location asrc)
{
    content.push_back({this, book::cat::message, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::debug        (std::source_location asrc)
{
    content.push_back({this, book::cat::debug, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::info         (std::source_location asrc)
{
    content.push_back({this, book::cat::info, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::comment      (std::source_location asrc)
{
    content.push_back({this, book::cat::comment, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::syntax       (std::source_location asrc)
{
    content.push_back({this, book::cat::syntax, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::status       (std::source_location asrc)
{
    content.push_back({this, book::cat::status, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::test         (std::source_location asrc)
{
    content.push_back({this, book::cat::test, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::interrupted  (std::source_location asrc)
{
    content.push_back({this, book::cat::interrupted, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::aborted      (std::source_location asrc)
{
    content.push_back({this, book::cat::aborted, asrc});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::segfault     (std::source_location asrc)
{
    content.push_back({this, book::cat::segfault, asrc});
    return content.back();
}






Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (Icon::Type graphem_code)
{
    text << graphem_code;
    input_components.push_back(text());
    text.clear();
    return *this;
}



Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (Accent::Type accent_code)
{
    text << accent_code;
    input_components.push_back(text());
    text.clear();
    return *this;
}


Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (const stracc& txt)
{
    text << txt;
    input_components.push_back(text());
    text.clear(); return *this;
}


Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (const std::string& txt)
{
    text << txt;
    input_components.push_back(text());
    text.clear();
    return *this;
}


Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (const char* txt)
{
    text << txt;
    input_components.push_back(text());
    text.clear();
    return *this;
}

Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (char c)
{
    text << c;
    input_components.push_back(text());
    text.clear();
    return *this;
}


Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (book::code c)
{
    text << book::code_text(c);
    input_components.push_back(text());
    text.clear();
    return *this;
}

Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (book::cat c)
{
    text << book::category_text(c);
    input_components.push_back(text());
    text.clear();
    return *this;

}


Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (color::code c)
{
    text << c;
    input_components.push_back(text());
    text.clear();
    return *this;
}


Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (const rect& dd)
{
    text << dd;
    input_components.push_back(text());
    text.clear();
    return *this;
}


Book::section::bloc_stack::element &Book::section::bloc_stack::element::operator <<(book::action tr)
{
    switch(tr)
    {
    case book::action::begin:
        break;
    case book::action::ci:
    case book::action::end:
        cc();
        parent<Book::section::bloc_stack>()->output_file << text();
        break;
    case book::action::enter:
        break;
    case book::action::leave:
        break;
    }

    return *this;
}


Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (Book::element_components cfg)
{
    ec = cfg;
    return *this;
}


Book::section::bloc_stack::element& Book::section::bloc_stack::element::operator << (book::functions tr)
{
    switch(tr)
    {
    case book::functions::endl:
        switch(Book::format()) ///@todo create Book::format(); directly instead.
        {
            case chattr::format::ansi256: text << "\n"; break;
            case chattr::format::html:    text << "<br />"; break;
        }
        break;
    case book::functions::stamp:
    {
        /*
             * %D %F ou %d %m %Y
             * %R %T %H %M %S
             * %A {Monday} %a {mon}
             * %B {June}   %b {jun}
        */


        //std::chrono::zoned_time date{"America/Toronto", std::chrono::system_clock::now()};
        //const auto tp{std::chrono::system_clock::now()};
        //auto txt{stracc::now("{:%H:%M:%S}", tp)};
        auto [ic,a] = book::function_attributes(book::functions::stamp);
        text << a.fg << Icon::Data[ic] << color::Reset << stracc::now("%T");
    }
    break;
    case book::functions::file:
        text << src.file_name();
        break;
    case book::functions::weekday:
    {
        auto [ic,a] = book::function_attributes(book::functions::weekday);
        //auto today{std::chrono::system_clock::now()};
        text << /*Icon::Data[ic] <<*/ a.fg << stracc::now("%A");
    }
        break;
    case book::functions::day :
    {
        auto [ic,a] = book::function_attributes(book::functions::day);
        //auto today{std::chrono::system_clock::now()};
        text << /*Icon::Data[ic] <<*/ a.fg << stracc::now("%d");
    }
        break;
    case book::functions::month:
    {
        auto [ic,a] = book::function_attributes(book::functions::month);
        //auto today{std::chrono::system_clock::now()};
        text << /*Icon::Data[ic] <<*/ a.fg << stracc::now("%m");
    }

    break;
    case book::functions::monthname:
    {
        auto [ic,a] = book::function_attributes(book::functions::month);
        //auto today{std::chrono::system_clock::now()};
        text << /*Icon::Data[ic] <<*/ a.fg << stracc::now("%B");
    }

    break;

    case book::functions::year:
    {
        auto [ic,a] = book::function_attributes(book::functions::year);
        //auto today{std::chrono::system_clock::now()};
        text << /*Icon::Data[ic] <<*/ a.fg << stracc::now("%Y");
    }
    break;
    case book::functions::function:
        text << src.function_name();
        break;
    default: break;
    }
    input_components.push_back(text());
    text.clear();
    return *this;
}





