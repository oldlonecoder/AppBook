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

/*

    Roadmap:
    Render rem/log entries in HTML.
    ex.:
    -- Log entry:
     <div class="log-entry">
        <span class="timestamp">...</span> ...
        ...
        <div class="log-line">
    </div>

*/



#pragma once

//#include <xio/appbook/>
#include <cstdint>
#include <string>
#include <string_view>
//#include <xio/appbook/utf/Icons.h>
//#include <xio/appbook/utf/accents.fr.h>
#include "chrtools/stracc.h"
#include "chrtools/geometry.h"
#include "logbook/public.h"

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




/**
@brief remark entry.

 ... Later, the rem class will no longer be managed using default static global stream as std::vector<std::string>.
 The Application Book API would rather take ownership and the
 control of the stream of rems as independant stream. So rem will only produce std::string

@author &copy;2023, Serge Lussier (serge.lussier@oldlonecoder.club)

 */
class BOOK_PUBLIC rem
{

    friend class application;
public:
    using code = uint16_t;
    using type = uint8_t;

    static constexpr type none              = 0;
    static constexpr type error             = 1;
    static constexpr type warning           = 2;
    static constexpr type fatal             = 3;
    static constexpr type except            = 4;
    static constexpr type message           = 5;
    static constexpr type output            = 6;
    static constexpr type debug             = 7;
    static constexpr type info              = 8;
    static constexpr type comment           = 9;
    static constexpr type syntax            = 10;
    static constexpr type status            = 11;
    static constexpr type test              = 12;
    static constexpr type interrupted       = 13;
    static constexpr type aborted           = 14;
    static constexpr type segfault          = 15;

    static constexpr code ok               = 0; ///< Obviously;
    static constexpr code accepted         = 1;
    static constexpr code success          = 2;
    static constexpr code rejected         = 3;
    static constexpr code failed           = 4;
    static constexpr code empty            = 5;
    static constexpr code full             = 6;
    static constexpr code notempty         = 7;
    static constexpr code implemented      = 8; ///< Like notimplemented or already implemented
    static constexpr code notimplemented   = 9; ///< "Please, implement"
    static constexpr code untested         = 10;
    static constexpr code eof              = 11; ///< end of file
    static constexpr code eos              = 12; ///< end of stream or string or statement or state ( machine state )
    static constexpr code null_ptr         = 13; ///< It will happen. Trust me :)
    static constexpr code endl             = 14; ///< end of line code, input format
    static constexpr code enter            = 15; ///< enter bloc or indent
    static constexpr code leave            = 16; ///< leaving bloc or unindent
    static constexpr code begin            = 17; ///< begin (sel)section or indent
    static constexpr code end              = 18; ///< end (logger: end of entry accumulators, so do commit); end of (sel)section, attribute ( auto- color::Reset ) and unindent
    static constexpr code notexist         = 19; ///< does not exist
    static constexpr code exist            = 20; ///< does already exist
    static constexpr code unexpected       = 21; ///< unexpected
    static constexpr code expected         = 22; ///< expected
    static constexpr code blocked          = 23; ///< thread trying to lock a mutex has failed because the mutex is already locked in another thread...
    static constexpr code locked           = 24; ///< thread trying to lock a mutex has became the owner of the lock.
    static constexpr code ci               = 25; ///< Commit action;
    static constexpr code stamp            = 26; ///< fully detailed timestamp
    static constexpr code overflow         = 27; ///< buffer overflow
    static constexpr code oob              = 28; ///< buffer overflow
    static constexpr code need_reimplement = 29;
    static constexpr code commit           = 30;
    static constexpr code function         = 1000;
    static constexpr code file             = 1001;
    static constexpr code line             = 1002;
    static constexpr code hour             = 4001;
    static constexpr code minute           = 4002;
    static constexpr code seconds          = 4003;
    static constexpr code weekday          = 4004;
private:
    using memory = std::vector<rem>;

    static rem::memory mem_stream;
    static chattr::format gencoding;
    stracc _text{};
    rem::code co = 0;
    rem::type ty = 0;
    int _indent = 0; ///< this rem entry indentation \see lsg::journal for how indentation is globally tracked and managed.
    static int _indentation;
    //-------------------- Colors and attributes Database -----------------------------------
    using attribute_data        = std::pair<Icon::Type, chattr::pair>;
    using type_attribute_pair   = std::pair<rem::type , rem::attribute_data>;
    using code_attribute_pair   = std::pair<rem::code , rem::attribute_data>;
    using type_attribute_table  = std::vector<rem::type_attribute_pair>;
    using code_attribute_table  = std::vector<rem::code_attribute_pair>;
    static rem::type_attribute_table types_data;
    static rem::code_attribute_table codes_data;
    //---------------------------------------------------------------------------------------
    source_location loc = {};
public:
    static std::string text(rem::type ty);
    static std::string text(rem::code co);

    rem() = default;
    ~rem();

    explicit rem(source_location&& sloc);
    rem(rem::type t, source_location&& sloc = {});
    rem(rem&&) noexcept = default;
    rem(const rem&) = default;
    rem& operator = (rem&&) noexcept = default;
    rem& operator = (const rem&) = default;

    rem& operator << (char c);
    rem& operator << (const char* c);
    rem& operator << (const std::string& str);
    rem& operator << (const stracc& str);
    rem& operator << (Icon::Type ic);
    rem& operator << (Accent::Type ac);
    rem& operator << (color::type c);
    rem& operator << (chattr::pair p);
    rem& operator << (const point& p);
    rem& operator << (const dim& dd);
    rem& operator << (const rect& dd);

    template<typename T> rem& operator << (const T& arg_)
    {
        _text << arg_;
        return *this;
    }

    rem& operator << (rem::code c);
    inline rem::code held_code() { return co; }
    inline rem::code held_type() { return ty; }

    static int indent();
    static int unindent();
    static int indentation();
    static rem& push_error(source_location&& src={});
    static rem& out(source_location&& src={});
    static rem& push_warning(source_location&& src={});
    static rem& push_fatal(source_location&& src={});
    static rem& push_except(source_location&& src={});
    static rem& push_message(source_location&& src={});
    static rem& push_debug(source_location&& src={});
    static rem& push_info(source_location&& src={});
    static rem& push_comment(source_location&& src={});
    static rem& push_syntax(source_location&& src={});
    static rem& push_status(source_location&& src={});
    static rem& push_test(source_location&& src={});
    static rem& push_interrupted(source_location&& src={});
    static rem& push_aborted(source_location&& src={});
    static rem& push_segfault(source_location&& src={});

    static rem::code init();

    static rem::attribute_data& attributes(rem::code c);
    static rem::attribute_data& attributes(rem::type t);
    std::string cc();
    static void clear(std::function<void(rem& re)> fn=nullptr);
    rem& tail();
    static rem::code assign_stream(std::fstream* cstream);


};



}
