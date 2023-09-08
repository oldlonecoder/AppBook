#include "logbook/Book.h"



Book* Book::_Book = nullptr;



using book::rem;


Book::Book()
{
    if(Book::_Book)
    {
        book::rem::push_error(HEREF) << " Attemps to instanciate singleton Book object - Forbidden!";
        return;
    }
    _Book = this;
    //...
}

Book::bloc* Book::log_scope()
{
    if(Book::_Book)
        return Book::_Book->current;
    book::rem::push_error(HERE) << "Using Book API without its instance";
    return nullptr;
}

Book::bloc& Book::new_bloc(const std::string& atitle)
{
    auto* b = new Book::bloc(atitle);
    Book::_Book->current = b;
    return *b;
}




Book::bloc::bloc(const std::string atitle): object(Book::log_scope(), atitle)
{

}

rem& Book::bloc::error(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::error,std::move(src));
    return content.back();
}


rem& Book::bloc::out(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::output,std::move(src));
    return content.back();
}


rem& Book::bloc::warning(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::warning,std::move(src));
    return content.back();
}

rem& Book::bloc::fatal(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::fatal,std::move(src));
    return content.back();
}

rem& Book::bloc::except(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::except,std::move(src));
    return content.back();
}

rem& Book::bloc::message(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::message,std::move(src));

    return content.back();
}

rem& Book::bloc::debug(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::debug,std::move(src));
    return content.back();
}

rem& Book::bloc::info(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::info,std::move(src));

    return content.back();
}

rem& Book::bloc::comment(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::comment,std::move(src));

    return content.back();
}

rem& Book::bloc::syntax(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::syntax,std::move(src));

    return content.back();
}

rem& Book::bloc::status(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::status,std::move(src));

    return content.back();
}

rem& Book::bloc::test(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::test,std::move(src));

    return content.back();
}

rem& Book::bloc::interrupted(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::interrupted,std::move(src));

    return content.back();
}

rem& Book::bloc::aborted(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::aborted,std::move(src));

    return content.back();
}

rem& Book::bloc::segfault(source_location&& src)
{
    if(!content.empty())
        content.back() << rem::endl;

    content.emplace_back(rem::segfault,std::move(src));

    return content.back();
}



