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



namespace book
{
<<<<<<< HEAD
=======

enum class cat : uint8_t{
    none, error, warning, fatal, except, message, output, debug, info, comment, syntax, status, test, interrupted, aborted, segfault
};
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91


enum class code : uint8_t
{
    ok               , ///< Obviously;
    accepted         ,
    success          ,
    rejected         ,
    failed           ,
    empty            ,
    full             ,
    notempty         ,
    implemented      , ///< Like notimplemented or already implemented
    notimplemented   , ///< "Please, implement"
    untested         ,
    eof              , ///< end of file
    eos              , ///< end of stream or string or statement or state ( machine state )
    null_ptr         , ///< It will happen. Trust me :)
    notexist         , ///< does not exist
    exist            , ///< does already exist
    unexpected       , ///< unexpected
    expected         , ///< expected
    blocked          , ///< thread trying to lock a mutex has failed because the mutex is already locked in another thread...
    locked           , ///< thread trying to lock a mutex has became the owner of the lock.
    overflow         , ///< buffer overflow
    oob              , ///< buffer overflow
    reimplement      ,

};

enum class functions : uint8_t
{
    function         ,
    endl             , ///< end of line code, input format
    file             ,
    line             ,
    hour             ,
    minute           ,
    seconds          ,
    weekday

};

enum class action : uint8_t
{
    enter            , ///< enter bloc or indent
    leave            , ///< end (logger: end of entry accumulators, so do commit); end of (sel)section, attribute ( auto- color::Reset ) and unindent
    ci               ,
    begin            , ///< begin (sel)section or indent
    end              ,
    stamp             ///< fully detailed timestamp
};

}

}

/*!
 * \brief The Book class
 *
 *  <p>
 *      <u> structure: </u>
 *      <ul>
 *          <li> book </li>
 *          <p>
 *              <ul>
 *                  <li>section : represented as (sub-)directory in the filesystem </li>
 *                  <ul>
 *                      <li>content-bloc: represented by file content under the section (sub-)directory</li>
 *                  </ul>
 *              <ul>
 *          </p>
 *      </ul>
 *  </P>
 *
 *
 * \code
 * Book;
 * //...
 * Book::error(HERE) << " Fonctionnement usuel de rem... " << rem::commit;
 * //...
 * \endcode
 * \author &copy;2023, oldlonecoder
 */

class BOOK_PUBLIC Book : public book::object
{

    std::ostream* out_stream{nullptr}; ///< Pointer to the address of the currently selected output stream.

    static Book* _Book; ///< Pointer to the Book instance.
    chattr::format _format{chattr::format::ansi256};
<<<<<<< HEAD
    std::string starting_path;
=======

>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
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

    };

    struct BOOK_PUBLIC config_data
    {
        std::string filename;
        chattr::format output_fmt;
        bool immediate;
    };

<<<<<<< HEAD

    static std::string category_text(book::cat c);
    static std::string ccode_text(book::code c);

=======
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91


    /*!
     * \brief The section class
     */
    struct BOOK_PUBLIC section : public book::object
    {

        using list = std::vector<Book::section*>;
        using iterator = list::iterator;

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
        struct BOOK_PUBLIC stack : public book::object
        {

            std::ofstream output_file; ///< the filename and path location are implicitely set by the parent section, this bloc ID and the ouput format.
            std::string description;


            using list = std::vector<Book::section::stack*>;
            using iterator = list::iterator;


            book::code commit(); ///< push element compiled text content into the output stream.

            std::string get_filename();


<<<<<<< HEAD
            struct BOOK_PUBLIC element : public book::object
=======
            struct BOOK_PUBLIC element
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
            {

                using memory = std::vector<Book::section::stack::element>;
                stracc     text;
<<<<<<< HEAD
                book::cat  category{book::cat::none};
=======
                book::cat  cat{book::cat::none};
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
                book::code code{book::code::rejected};
                book::source_location src{};

                element() = default;
<<<<<<< HEAD
                element(const element&) = default;
                element(element&& e) noexcept = default;

                element(book::object* par, book::cat category, book::source_location&& asrc);

                Book::section::stack::element& operator = (const element& e) = default;
                Book::section::stack::element& operator = (element&& e) noexcept = default;
=======
                element(const element&);
                element(element&& e) noexcept;

                Book::section::stack::element& operator = (const element& e);
                Book::section::stack::element& operator = (element&& e) noexcept;
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91

                Book::section::stack::element& operator << (Icon::Type graphem);
                Book::section::stack::element& operator << (Accent::Type accent);
                Book::section::stack::element& operator << (const stracc& txt);
                Book::section::stack::element& operator << (const std::string& txt);
                Book::section::stack::element& operator << (const char* txt);

                Book::section::stack::element& operator << (color::code c);
                Book::section::stack::element& operator << (chattr::pair p);
                Book::section::stack::element& operator << (const point& p);
                Book::section::stack::element& operator << (const dim& dd);
                Book::section::stack::element& operator << (const rect& dd);

                Book::section::stack::element& operator << (book::action tr);
                Book::section::stack::element& operator << (book::functions tr);


                template<typename other_types> Book::section::stack::element& operator << (other_types val);

                std::string cc();
                book::code commit();


            };


            Book::section::stack::element::memory content;

            stack(const std::string atitle);
            stack(book::object* parent_obj /* Normally Book::section* */, const std::string& atitle);

            ~stack() override;

<<<<<<< HEAD
            book::code open();///< If exist, open in append mode. or create if not exist.
=======
            book::code open();


            book::expect<> open();///< If exist, open in append mode. or create if not exist.
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91

            Book::section::stack::element& error        (book::source_location&& src={});
            Book::section::stack::element& out          (book::source_location&& src={});
            Book::section::stack::element& warning      (book::source_location&& src={});
            Book::section::stack::element& fatal        (book::source_location&& src={});
            Book::section::stack::element& except       (book::source_location&& src={});
            Book::section::stack::element& message      (book::source_location&& src={});
            Book::section::stack::element& debug        (book::source_location&& src={});
            Book::section::stack::element& info         (book::source_location&& src={});
            Book::section::stack::element& comment      (book::source_location&& src={});
            Book::section::stack::element& syntax       (book::source_location&& src={});
            Book::section::stack::element& status       (book::source_location&& src={});
            Book::section::stack::element& test         (book::source_location&& src={});
            Book::section::stack::element& interrupted  (book::source_location&& src={});
            Book::section::stack::element& aborted      (book::source_location&& src={});
            Book::section::stack::element& segfault     (book::source_location&& src={});
        };

        std::string title;
        std::string description;

        section(const std::string& section_id);
        ~section() override;

        stack* create_bloc(const std::string& bloc_id);
        stack::list blocs;

    };




    ~Book() override;



    config_data& config() { return conf; }

<<<<<<< HEAD
    book::code open();
    book::code close();
=======
    book::rem::code open();
    book::rem::code close();
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91

    Book::section* operator[](std::string_view section_id);

    // held back :
    //static book::rem& error(source_location&& src = {});
    //static book::rem& out(source_location&& src = {});
    //static book::rem& warning(source_location&& src = {});
    //static book::rem& fatal(source_location&& src = {});
    //static book::rem& except(source_location&& src = {});
    //static book::rem& message(source_location&& src = {});
    //static book::rem& debug(source_location&& src = {});
    //static book::rem& info(source_location&& src = {});
    //static book::rem& comment(source_location&& src = {});
    //static book::rem& syntax(source_location&& src = {});
    //static book::rem& status(source_location&& src = {});
    //static book::rem& test(source_location&& src = {});
    //static book::rem& interrupted(source_location&& src = {});
    //static book::rem& aborted(source_location&& src = {});
    //static book::rem& segfault(source_location&& src = {});


<<<<<<< HEAD
    class BOOK_PUBLIC exception: public std::exception
=======
    class exception: public std::exception
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
    {
    public:
        explicit exception(const char* txt) { msg = txt; }
        explicit exception(const std::string& txt) { msg = txt; }
        ~exception() noexcept { msg.clear(); }

        const char* what() const noexcept override { return msg.c_str(); }

    private:
        std::string msg;
    };

<<<<<<< HEAD
    static Book& init(const std::string& book_name);
    static Book& Self();

private:
=======
    static book::code init(const std::string& book_name);
private:
    Book();
    Book(const std::string& book_id);
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
    section::list sections;
    Book::section::stack* current_stream{nullptr};
    config_data conf;
    Book::element_components ec{0};
    static Book* __Application_Book__;
<<<<<<< HEAD

    Book();
    Book(const std::string& book_id);

=======
>>>>>>> 1561d168332079b45d8ad16fb3928d3e2e14cd91
};





