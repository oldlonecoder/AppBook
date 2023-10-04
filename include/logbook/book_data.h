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

#include <chrtools/stracc.h>
#include <logbook/public.h>

namespace book
{


struct  BOOK_PUBLIC source_location
{
    int line = 0;
    std::string_view filename = "";
    std::string_view function_sig = "";
    explicit operator bool()const { return !((line == 0) && filename.empty() && function_sig.empty()); }
};

//#define sfnl    {__LINE__,__FILE__,__FUNCTION__}
//#define sfnll   {__LINE__,__FILE__,__PRETTY_FUNCTION__}
//#define sfl     {__LINE__, "", __FUNCTION__}
#define HERE    {__LINE__,__FILE__,__FUNCTION__}
#define HEREF   {__LINE__,__FILE__,__PRETTY_FUNCTION__}




enum class cat : uint8_t{
    none, error, warning, fatal, except, message, output, debug, info, comment, syntax, status, test, interrupted, aborted, segfault
};


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
    stamp            ,
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
    ///< fully detailed timestamp
};


std::string category_text(book::cat c);
std::string code_text(book::code c);

std::pair<Icon::Type, chattr::pair> category_attributes(book::cat cat);
std::pair<Icon::Type, chattr::pair> code_attributes(book::code c);
std::pair<Icon::Type, chattr::pair> function_attributes(book::functions f);

}
