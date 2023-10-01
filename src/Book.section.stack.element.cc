#include "logbook/Book.h"




Book::section::stack::element::element(book::cat category, book::source_location &&asrc):book::object()
{
    src = std::move(asrc);
    this->category = category;
    code = book::code::rejected;
}

Book::section::stack::element &Book::section::stack::element::operator <<(book::action tr)
{
    switch(tr)
    {
        case book::action::begin:
        break;
        case book::action::ci:
            parent<Book::section::stack>()->output_file << cc();

        break;
        case book::action::end:
        break;
        case book::action::enter:
        break;
        case book::action::leave:
        break;
    }

    return *this;
}




Book::section::stack::element& Book::section::stack::error        (book::source_location&& asrc)
{
    content.push_back({this, book::cat::error, std::move(asrc)});
    return content.back();
}

Book::section::stack::element& Book::section::stack::out          (book::source_location&& asrc)
{
    content.push_back({this, book::cat::output, std::move(asrc)});
    return content.back();
}

Book::section::stack::element& Book::section::stack::warning      (book::source_location&& asrc)
{
    content.push_back({this, book::cat::warning, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::fatal        (book::source_location&& asrc)
{
    content.push_back({this, book::cat::fatal, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::except       (book::source_location&& asrc)
{
    content.push_back({this, book::cat::except, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::message      (book::source_location&& asrc)
{
    content.push_back({this, book::cat::message, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::debug        (book::source_location&& asrc)
{
    content.push_back({this, book::cat::debug, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::info         (book::source_location&& asrc)
{
    content.push_back({this, book::cat::info, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::comment      (book::source_location&& asrc)
{
    content.push_back({this, book::cat::comment, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::syntax       (book::source_location&& asrc)
{
    content.push_back({this, book::cat::syntax, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::status       (book::source_location&& asrc)
{
    content.push_back({this, book::cat::status, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::test         (book::source_location&& asrc)
{
    content.push_back({this, book::cat::test, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::interrupted  (book::source_location&& asrc)
{
    content.push_back({this, book::cat::interrupted, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::aborted      (book::source_location&& asrc)
{
    content.push_back({this, book::cat::aborted, std::move(asrc)});
    return content.back();
}


Book::section::stack::element& Book::section::stack::segfault     (book::source_location&& asrc)
{
    content.push_back({this, book::cat::segfault, std::move(asrc)});
    return content.back();
}


