#include "logbook/Book.h"
#include <chrtools/strbrk.h>



Book::section::bloc_stack::element::element(object* par, book::cat category, std::source_location &&asrc):book::object(par,"q-anon element!")
{
    src = std::move(asrc);
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


Book::section::bloc_stack::element& Book::section::bloc_stack::error        (std::source_location&& asrc)
{
    content.push_back({this, book::cat::error, std::move(asrc)});
    return content.back();
}

Book::section::bloc_stack::element& Book::section::bloc_stack::out          (std::source_location&& asrc)
{
    content.push_back({this, book::cat::output, std::move(asrc)});
    return content.back();
}

Book::section::bloc_stack::element& Book::section::bloc_stack::warning      (std::source_location&& asrc)
{
    content.push_back({this, book::cat::warning, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::fatal        (std::source_location&& asrc)
{
    content.push_back({this, book::cat::fatal, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::except       (std::source_location&& asrc)
{
    content.push_back({this, book::cat::except, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::message      (std::source_location&& asrc)
{
    content.push_back({this, book::cat::message, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::debug        (std::source_location&& asrc)
{
    content.push_back({this, book::cat::debug, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::info         (std::source_location&& asrc)
{
    content.push_back({this, book::cat::info, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::comment      (std::source_location&& asrc)
{
    content.push_back({this, book::cat::comment, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::syntax       (std::source_location&& asrc)
{
    content.push_back({this, book::cat::syntax, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::status       (std::source_location&& asrc)
{
    content.push_back({this, book::cat::status, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::test         (std::source_location&& asrc)
{
    content.push_back({this, book::cat::test, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::interrupted  (std::source_location&& asrc)
{
    content.push_back({this, book::cat::interrupted, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::aborted      (std::source_location&& asrc)
{
    content.push_back({this, book::cat::aborted, std::move(asrc)});
    return content.back();
}


Book::section::bloc_stack::element& Book::section::bloc_stack::segfault     (std::source_location&& asrc)
{
    content.push_back({this, book::cat::segfault, std::move(asrc)});
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
        text << stracc::now("%H:%M:%DS");
        break;
    case book::functions::file:
        text << src.file_name();
        break;
    case book::functions::weekday:
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





