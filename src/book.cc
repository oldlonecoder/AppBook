#include "logbook/Book.h"


using book::rem;


Book::Book()
{

}

rem& Book::error(source_location&& src)
{
    return rem::push_error(std::move(src));
}

rem& Book::out(source_location&& src)
{

}

rem& Book::warning(source_location&& src)
{

}

rem& Book::fatal(source_location&& src)
{

}

rem& Book::except(source_location&& src)
{

}

rem& Book::message(source_location&& src)
{

}

rem& Book::debug(source_location&& src)
{

}

rem& Book::info(source_location&& src)
{

}

rem& Book::comment(source_location&& src)
{

}

rem& Book::syntax(source_location&& src)
{

}

rem& Book::status(source_location&& src)
{

}

rem& Book::test(source_location&& src)
{

}

rem& Book::interrupted(source_location&& src)
{

}

rem& Book::aborted(source_location&& src)
{

}

rem& Book::segfault(source_location&& src)
{

}



