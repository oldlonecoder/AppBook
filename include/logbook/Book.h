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
//#include <chrtools/textattr.h>

using book::source_location;

class BOOK_PUBLIC Book
{

    std::ofstream* out_stream{nullptr}; ///< Output stream address.


public:

    struct BOOK_PUBLIC config_data
    {
        std::string filename;
        chattr::format output_fmt;
        bool immediate;
    };

    Book();

    config_data& config() { return conf; }

    static book::rem& error(source_location&& src={});
    static book::rem& out(source_location&& src={});
    static book::rem& warning(source_location&& src={});
    static book::rem& fatal(source_location&& src={});
    static book::rem& except(source_location&& src={});
    static book::rem& message(source_location&& src={});
    static book::rem& debug(source_location&& src={});
    static book::rem& info(source_location&& src={});
    static book::rem& comment(source_location&& src={});
    static book::rem& syntax(source_location&& src={});
    static book::rem& status(source_location&& src={});
    static book::rem& test(source_location&& src={});
    static book::rem& interrupted(source_location&& src={});
    static book::rem& aborted(source_location&& src={});
    static book::rem& segfault(source_location&& src={});

private:
    config_data conf;


};



