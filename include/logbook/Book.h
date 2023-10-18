/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once
//#include <logbook/expect.h>
#include <fstream>
#include <logbook/object.h>
#include <chrtools/stracc.h>
#include <chrtools/geometry.h>
#include <chrtools/accents.fr.h>
#include <chrtools/Icons.h>
#include <logbook/book_data.h>
#include <filesystem>
#include <memory> //< Check if a smart pointer could delete Book::__APplication_Book__ preperly.



/*!
 * \brief The Book class



 * \author &copy;2023, oldlonecoder
 */

class BOOK_PUBLIC Book : public book::object
{

    std::ostream* out_stream{nullptr}; ///< Pointer to the address of the currently selected output stream.
    chattr::format _format{chattr::format::ansi256};
    std::string starting_path;

public:

    struct element_components
    {
        uint16_t icon     : 1;
        uint16_t filename : 1;
        uint16_t line     : 1;
        uint16_t column   : 1;
        uint16_t funcname : 1;
        uint16_t funcsig  : 1;
        uint16_t stamp    : 1;
        uint16_t time     : 1;
        uint16_t day      : 1;
        uint16_t month    : 1;
        uint16_t year     : 1;
        //...
        using list = std::map<std::string, Book::element_components>;
    };

    static Book::element_components::list header_components_db;


    struct BOOK_PUBLIC config_data
    {
        std::string filename;
        [[maybe_unused]] chattr::format output_fmt;
        [[maybe_unused]] bool immediate;
    };

    /*!
     * \brief The section class
     */
    struct BOOK_PUBLIC section : public book::object
    {

        using list = std::vector<Book::section*>;
        using iterator [[maybe_unused]] = list::iterator;

        /*!
        * \brief The bloc class
        *
        * \code // ex.:
        *
        *    //...
        *    Book::bloc* aro = Book::instance["Runtime"]["arithmetics-dev"];
        *    //...
        *    if(alu_fail)
        *        throw aro.except(HERE) << " alu: division by zero";
        *    //...
        *    aro.debug(HERE)(.../xio.cc line 340; function: collapse_closepair( xio* ) ) << " debug trace...";
        *
        *    //...
        *    Book::select("section-id/bloc-id");
        *    ----------------------------------------- :>
        *
        *
        *
        *    // ...
        *
        *    Book::out() << " salutation!";
        *
        *
        *
        */
        struct BOOK_PUBLIC bloc_stack : public book::object
        {

            std::ofstream output_file; ///< the filename and path location are implicitely set by the parent section, this bloc ID and the ouput format.
            std::string description;
            std::string filename;
            std::filesystem::path location;
            using list = std::vector<Book::section::bloc_stack*>;
            using iterator = list::iterator;


            book::code commit(); ///< push element compiled text content into the output stream.

            std::string get_filename();



            struct BOOK_PUBLIC element : public book::object
            {

                using memory = std::vector<Book::section::bloc_stack::element>;

                std::vector<std::string> input_components;

                stracc     text;
                book::cat  cat{book::cat::none};

                book::code code{book::code::rejected};
                std::source_location src{};

                Book::element_components  ec{0};
                element() = default;

                element(const element&) = default;
                element(element&& e) noexcept = default;

                element(book::object* par, book::cat category, std::source_location asrc);

                Book::section::bloc_stack::element& operator = (const element& e) = default;
                Book::section::bloc_stack::element& operator = (element&& e) noexcept = default;

                Book::section::bloc_stack::element& operator << (Icon::Type graphem);
                Book::section::bloc_stack::element& operator << (Accent::Type accent);
                Book::section::bloc_stack::element& operator << (const stracc& txt);
                Book::section::bloc_stack::element& operator << (const std::string& txt);
                Book::section::bloc_stack::element& operator << (const char* txt);
                Book::section::bloc_stack::element& operator << (char c);
                Book::section::bloc_stack::element& operator << (book::code c);
                Book::section::bloc_stack::element& operator << (book::cat c);
                Book::section::bloc_stack::element& operator << (color::code c);
                Book::section::bloc_stack::element& operator << (const rect& dd);
                Book::section::bloc_stack::element& operator << (book::action tr);
                Book::section::bloc_stack::element& operator << (book::functions tr);
                Book::section::bloc_stack::element& operator << (Book::element_components cfg);

                template<typename other_types> Book::section::bloc_stack::element& operator << (other_types val)
                {
                    text << val;
                    input_components.push_back(text());
                    text.clear();
                    return *this;
                }
                void cc_header();
                book::code cc();
                book::code commit();
            };

            bloc_stack() = default;
            Book::section::bloc_stack::element::memory content;
            bloc_stack(book::object* parent_obj /* Normally Book::section* */, const std::string& atitle);

            ~bloc_stack() override;

            book::code open();///< If exist, open in append mode. or create if not exist.
            book::code close();


            Book::section::bloc_stack::element& error        (std::source_location src);
            Book::section::bloc_stack::element& out          (std::source_location src);
            Book::section::bloc_stack::element& warning      (std::source_location src);
            Book::section::bloc_stack::element& fatal        (std::source_location src);
            Book::section::bloc_stack::element& except       (std::source_location src);
            Book::section::bloc_stack::element& message      (std::source_location src);
            Book::section::bloc_stack::element& debug        (std::source_location src);
            Book::section::bloc_stack::element& info         (std::source_location src);
            Book::section::bloc_stack::element& comment      (std::source_location src);
            Book::section::bloc_stack::element& syntax       (std::source_location src);
            Book::section::bloc_stack::element& status       (std::source_location src);
            Book::section::bloc_stack::element& test         (std::source_location src);
            Book::section::bloc_stack::element& interrupted  (std::source_location src);
            Book::section::bloc_stack::element& aborted      (std::source_location src);
            Book::section::bloc_stack::element& segfault     (std::source_location src);
        };

        std::string title;
        std::string description;

        std::filesystem::path location;


        section(object* par, const std::string& section_id);
        ~section() override;
        book::code close();
        Book::section& open();


        bloc_stack& create_stack(const std::string& stack_id);
        bloc_stack::list blocs;

        Book::section::bloc_stack& operator[](const std::string& bloc_id);

    };

    class BOOK_PUBLIC exception: public std::exception
    {
    public:
        explicit exception(const char* txt) { msg = txt; }
        explicit exception(const std::string& txt) { msg = txt; }
        ~exception() noexcept  override { msg.clear(); }

        [[nodiscard]] const char* what() const noexcept override { return msg.c_str(); }

    private:
        std::string msg;
    };





    ~Book() override;



    config_data& config() { return conf; }


    book::code open();
    static book::code close();

    std::string descriptions;

    Book::section& operator[](std::string_view section_id);
    Book::section& create_section(const std::string& section_id);

    static chattr::format format();


    static Book& init(const std::string& book_name);
    static Book& Self();
    static std::filesystem::path location;



    static Book::section::bloc_stack::element& error       (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& out         (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& warning     (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& fatal       (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& except      (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& message     (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& debug       (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& info        (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& comment     (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& syntax      (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& status      (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& test        (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& interrupted (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& aborted     (std::source_location src=std::source_location::current());
    static Book::section::bloc_stack::element& segfault    (std::source_location src=std::source_location::current());

    static book::code commit();

//    --- held back ...
//    static std::string today();
//    static std::string date();
//    static std::string time(const std::string& tz="America/Toronto"); ///< Just for testing
//    static std::string year();
//    ------------------------------


private:
    explicit Book(const std::string& book_id);
    Book();

    section::list sections;
    Book::section::bloc_stack* current_stream{nullptr};
    config_data conf;
    Book::element_components ec{0};
    static Book* Application_Book;

};





