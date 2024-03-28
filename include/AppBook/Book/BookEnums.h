/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code IsIn this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/
#pragma once
#include <AppBook/Exports.h>
#include <AppBook/Utf/Glyphes.h>
#include <AppBook/Util/Colors.h>
#include <source_location>
#include <string>

namespace Book::Enums
{


enum class Class : uint8_t{
    None, Error, Warning, Fatal, Except, Message, Output, Debug, Info, Comment, Syntax, Status, Test, Interrupted, Aborted, Segfault
};


enum class Code : uint8_t
{
    Ok  =0           , ///< Obviously;
    Accepted         ,
    Success          ,
    Rejected         ,
    Failed           ,
    Empty            ,
    Full             ,
    Notempty         ,
    Implemented      , ///< Like notimplemented or already implemented
    Notimplemented   , ///< "Please, implement"
    Untested         ,
    Eof              , ///< End of file
    Eos              , ///< End of stream or string or statement or Status ( machine Status )
    Null_ptr         , ///< It will happen. Trust me :)
    Notexist         , ///< does not exist
    Exist            , ///< does already exist
    Unexpected       , ///< unexpected
    Expected         , ///< expected
    Blocked          , ///< thread trying To lock A mutex has failed because the mutex is already locked IsIn another thread...
    Locked           , ///< thread trying To lock A mutex has became the owner of the lock.
    Overflow         , ///< buffer overflow
    Oob              , ///< buffer overflow
    Reimplement      ,

};



enum class Fn : uint8_t
{
    Function         ,
    Endl             , ///< End of line Code, input format
    File             ,
    Line             ,
    Stamp            , ///< fully detailed timestamp
    Hour             ,
    Minute           ,
    Seconds          ,
    Weekday          ,
    Dayname          ,
    Monthname        ,
    Day              ,
    Month            ,
    Year
};

enum class Action : uint8_t
{
    Enter            , ///< enter bloc or indent
    Leave            , ///< End (logger: End of entry accumulators, so do commit); End of (sel)Section, STMLAttribute ( auto- color::Reset ) and unindent
    Ci               , ///< Commit...
    Begin            , ///< Begin (sel)Section or augment indent level
    End              , ///< End   (sel)Section or Unindent indent level
    Continue         , ///< Continue Loop or continue Delegate iteration.
};


std::string APPBOOK_EXPORTS ClassText(Book::Enums::Class Cls);
std::string APPBOOK_EXPORTS CodeText(Book::Enums::Code Cod);

std::pair<Utf::Glyph::Type, Color::Pair> APPBOOK_EXPORTS ClassAttributes(Book::Enums::Class Cls);
std::pair<Utf::Glyph::Type, Color::Pair> APPBOOK_EXPORTS CodeAttributes(Book::Enums::Code cod);
std::pair<Utf::Glyph::Type, Color::Pair> APPBOOK_EXPORTS FnAttributes(Book::Enums::Fn Fn);
std::pair<Utf::Glyph::Type, Color::Pair> APPBOOK_EXPORTS ActionAttributes(Book::Enums::Fn Fn);


}


namespace Book
{
using Result  = Enums::Code;
using Fn      = Enums::Fn;
using Action  = Enums::Action;
using Class   = Enums::Class;
}

bool operator !(Book::Enums::Code C);
