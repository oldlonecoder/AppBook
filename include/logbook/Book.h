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
#include <logbook/rem.h>
#include <fstream>
#include <logbook/object.h>

//#include <chrtools/textattr.h>

using book::source_location;




/*!
 * \brief The Book class
 *
 *
 *
 * \code
 * Book;
 * //...
 * Book::error(HERE) << " Fonctionnement usuel de rem... " << rem::commit;
 */

class BOOK_PUBLIC Book
{

    std::ofstream* out_stream{nullptr}; ///< Output stream address.

    static Book* _Book;
    chattr::format _format{chattr::format::ansi256};


public:

    struct BOOK_PUBLIC config_data
    {
        std::string filename;
        chattr::format output_fmt;
        bool immediate;
    };

    /*!
     * \brief The bloc class
     *
     * \note ex.: Book["section::subject"]["bloc-id?"].error(HERE) << "...";
     *    //...
     *    Book::bloc& aro = Book::address["Runtime"]["arithmetics-dev"];
     *    //...
     *    if(alu_fail)
     *        throw aro.except(HERE) << " alu: division by zero";
     *    //...
     *    aro.debug(HERE)(.../xio.cc line 340; function: collapse_closepair( xio* ) ) << " debug trace...";
     *
     *    //...
     *    Book::select("section-id/bloc-id);
     *    Book::out() << " salutation!";
     *
     *    Enlever Book::bloc qui est superflu. Déplacer bloc à-même la section - donc bloc devient Book::Section.
     */
    struct BOOK_PUBLIC bloc :public book::object
    {
        book::rem::memory content;
        bloc(const std::string atitle);
        bloc(book::object* parent_obj /* Normally Book::section* */, const std::string& atitle);

        ~bloc() override;

        book::rem& error        (source_location&& src={});
        book::rem& out          (source_location&& src={});
        book::rem& warning      (source_location&& src={});
        book::rem& fatal        (source_location&& src={});
        book::rem& except       (source_location&& src={});
        book::rem& message      (source_location&& src={});
        book::rem& debug        (source_location&& src={});
        book::rem& info         (source_location&& src={});
        book::rem& comment      (source_location&& src={});
        book::rem& syntax       (source_location&& src={});
        book::rem& status       (source_location&& src={});
        book::rem& test         (source_location&& src={});
        book::rem& interrupted  (source_location&& src={});
        book::rem& aborted      (source_location&& src={});
        book::rem& segfault     (source_location&& src={});
    };


    /*!
     * \brief The section class
     */
    struct BOOK_PUBLIC section : public book::object
    {
        section(const std::string& atitle);
        ~section() override;

        Book::bloc& new_bloc(const std::string& atitle);



    };


    Book();

    config_data& config() { return conf; }

    static Book::bloc* log_scope();
    static Book::bloc& new_bloc(const std::string& atitle);


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

private:
    config_data conf;

    Book::bloc* current{nullptr};

};





