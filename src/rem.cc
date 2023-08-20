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


#include "logbook/rem.h"
#include "chrtools/strbrk.h"
#include <mutex>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   pragma message("Warning: This is the development version.")
#else 
#   warning "This is the development version."
#endif


namespace book
{

std::mutex guard{};

rem::memory rem::mem_stream;
int rem::_indentation = 0;


// push_*(id, source_location) -> mem_stream[id].emplace_back(...) ;
// ... auto_fush periodically in background

rem::type_attribute_table rem::types_data = {
    {rem::none,        {Icon::Computer,    {color::Aquamarine1         ,color::Reset }}},
    {rem::error,       {Icon::Err1,        {color::Red4                ,color::Reset }}},
    {rem::warning,     {Icon::Warning,     {color::Yellow              ,color::Reset }}},
    {rem::fatal,       {Icon::DeadHead,    {color::HotPink4            ,color::Reset }}},
    {rem::except,      {Icon::Bolt,        {color::SkyBlue3            ,color::Reset }}},
    {rem::message,     {Icon::Comment,     {color::Green4              ,color::Reset }}},
    {rem::output,      {Icon::NotePad,     {color::Grey39              ,color::Reset }}},
    {rem::debug,       {Icon::Bug,         {color::Pink3               ,color::Reset }}},
    {rem::info,        {Icon::Info,        {color::LightSkyBlue4       ,color::Reset }}},
    {rem::comment,     {Icon::Comment,     {color::Grey42              ,color::Reset }}},
    {rem::syntax,      {Icon::Handwrite,   {color::LighcoreateBlue     ,color::Reset }}},
    {rem::status,      {Icon::Admin,       {color::LightCyan3          ,color::Reset }}},
    {rem::test,        {Icon::File,        {color::Aquamarine3         ,color::Reset }}},
    {rem::interrupted, {Icon::CircleX,     {color::Khaki3              ,color::Reset }}},
    {rem::aborted,     {Icon::Stop,        {color::Red4                ,color::Reset }}},
    {rem::segfault,    {Icon::Bomb,        {color::SandyBrown          ,color::Reset }}}
    //...
};

rem::code_attribute_table rem::codes_data = {
    {rem::ok,              {Icon::ThumbUp,         {color::DarkGreen         ,color::Reset }}},
    {rem::accepted,        {Icon::Success,         {color::DarkGreen         ,color::Reset }}},
    {rem::success,         {Icon::Success,         {color::DarkGreen         ,color::Reset }}},
    {rem::rejected,        {Icon::ThumbDown,       {color::Orange5           ,color::Reset }}},
    {rem::failed,          {Icon::Poop,            {color::DarkGoldenrod     ,color::Reset }}},
    {rem::empty,           {Icon::ArrowRight,      {color::LighcoreateGrey   ,color::Reset }}},
    {rem::full,            {Icon::SmallDot,        {color::White             ,color::Reset }}},
    {rem::notempty,        {Icon::BigDot,          {color::White             ,color::Reset }}},
    {rem::implemented,     {Icon::ArrowHeadRight,  {color::Lime              ,color::Reset }}},
    {rem::notimplemented,  {Icon::Err1,            {color::Orange3           ,color::Reset }}},
    {rem::untested,        {Icon::Flag,            {color::Yellow            ,color::Reset }}},
    {rem::eof,             {Icon::Baseline,        {color::White             ,color::Reset }}},
    {rem::eos,             {Icon::Baseline,        {color::White             ,color::Reset }}},
    {rem::null_ptr,        {Icon::CircleX,         {color::HotPink4          ,color::Reset }}},
    {rem::endl,            {Icon::CircleX,         {color::HotPink4          ,color::Reset }}},
    {rem::enter,           {Icon::KeyEnter,        {color::Green4            ,color::Reset }}},
    {rem::leave,           {Icon::Enter,           {color::Green4            ,color::Reset }}},
    {rem::begin     ,      {Icon::Star5 ,          {color::White             ,color::Reset }}},
    {rem::end       ,      {Icon::Downtobottomleft,{color::White             ,color::Reset }}},
    {rem::notexist  ,      {Icon::CircleX ,        {color::White             ,color::Reset }}},
    {rem::exist     ,      {Icon::Star5 ,          {color::White             ,color::Reset }}},
    {rem::unexpected,      {Icon::Flag ,           {color::Yellow            ,color::Reset }}},
    {rem::expected  ,      {Icon::Cat ,            {color::White             ,color::Reset }}},
    {rem::blocked   ,      {Icon::UnAuth ,         {color::IndianRed3        ,color::Reset }}},
    {rem::locked    ,      {Icon::Err3 ,           {color::White             ,color::Reset }}},
    {rem::stamp     ,      {Icon::Chronos ,        {color::SkyBlue3          ,color::Reset }}},
    {rem::function  ,      {Icon::Function ,       {color::SkyBlue3          ,color::Reset }}},
    {rem::file      ,      {Icon::File ,           {color::White             ,color::Reset }}},
    {rem::line      ,      {Icon::Baseline ,       {color::White             ,color::Reset }}},
    {rem::stamp     ,      {Icon::Chronos ,        {color::Yellow            ,color::Reset }}},
    {rem::overflow  ,      {Icon::DeadHead,        {color::Red4              ,color::Reset }}},
    {rem::oob       ,      {Icon::Alien,           {color::Red4              ,color::Reset }}},
    {rem::need_reimplement,{Icon::Books,           {color::Yellow            ,color::Reset }}},
    {rem::hour      ,      {Icon::Chronos ,        {color::White             ,color::Reset }}},
    {rem::minute    ,      {Icon::Chronos ,        {color::White             ,color::Reset }}},
    {rem::seconds   ,      {Icon::Chronos ,        {color::White             ,color::Reset }}},
    {rem::weekday   ,      {Icon::Chronos ,        {color::White             ,color::Reset }}}
};





std::string rem::text(rem::type ty)
{
    std::map<rem::type, const char*> _ = {
        {error  , "error"   },
        {warning, "warning" },
        {fatal  , "fatal"   },
        {except , "except"  },
        {message, "message" },
        {output , "output"  },
        {debug  , "debug"   },
        {info   , "info"    },
        {comment, "comment" },
        {syntax , "syntax"  },
        {status , "status"  },
        {test   , "test"    }
    };
    return _[ty];
}

std::map<rem::code, const char*> _ = {
    {rem::ok,             "ok"},
    {rem::accepted,       "accepted"},
    {rem::success,        "success "},
    {rem::rejected,       "rejected"},
    {rem::failed,         "failed"},
    {rem::empty,          "empty"},
    {rem::full,           "full"},
    {rem::notempty,       "notempty"},
    {rem::implemented,    "implemented"},
    {rem::notimplemented, "notimplemented"},
    {rem::untested,       "untested"},
    {rem::eof,            "eof"},
    {rem::eos,            "eos"},
    {rem::null_ptr,       "null_ptr"},
    {rem::endl,           "end line"},
    {rem::enter,          "enter"},
    {rem::leave,          "leave"},
    {rem::begin,          "begin"},
    {rem::end,            "end"},
    {rem::notexist,       "notexist"},
    {rem::exist,          "exist"},
    {rem::unexpected,     "unexpected"},
    {rem::expected,       "expected"},
    {rem::blocked,        "blocked"},
    {rem::locked,         "locked"},
    {rem::function,       "function"},
    {rem::file,           "file"},
    {rem::line,           "line"},
    {rem::stamp,          "stamp"},
    {rem::overflow,       "buffer overflow"},
    {rem::oob,            "out of boundaries"},
    {rem::need_reimplement,"need to be re-implemented in derived instances"},
    {rem::hour,           "hour"},
    {rem::minute,         "minute"},
    {rem::seconds,        "seconds"},
    {rem::weekday,        "weekday"}
};

std::string rem::text(rem::code co)
{

    return _[co];
}

rem::rem(rem::type aty, source_location &&_l):ty(aty), loc(_l)
{
    _indent = rem::_indentation;
}


int rem::indent()
{
    return ++rem::_indentation;
}

int rem::unindent()
{
    if(!rem::_indentation)
        return rem::_indentation;

    return --rem::_indentation;
}

int rem::indentation()
{
    return _indentation;
}
/*
rem::rem(rem &&le)
    noexcept {
    loc = stdle.loc;
    ty = le.ty;
    co = le.co;
    _indent = le._indent;
    _text = std::move(le._text);
}

rem::rem(const rem &le)
{
    loc = le.loc;
    ty = le.ty;
    co = le.co;
    _indent = le._indent;
    _text = le._text;

}



rem& rem::operator=(rem& &le) noexcept
{
    loc = le.loc;
    typ = le.typ;
    cod = le.cod;
    indent = le.indent;
    str_acc = le.str_acc;
    return *this;
}

rem& rem::operator=(const rem& le)
{
    loc = le.loc;
    typ = le.ty;
    cod = le.co;
    _indent = le._indent;
    _text = le._text;
    return *this;
}
*/


rem::~rem()
{
    _text.clear();
}


rem& rem::operator << (char c)
{
    //guard.lock();
    _text << c;
    //guard.unlock();
    return *this;
}

rem& rem::operator << (const char* c)
{
    //guard.lock();
    _text << c;
    //guard.unlock();
    return *this;
}


rem& rem::operator << (const std::string& str) {  /*guard.lock();*/ _text << str; /* guard.unlock();*/ return *this; }
rem& rem::operator << (const stracc& str) {   _text << str; return *this; }
rem& rem::operator << (Icon::Type ic) {  _text << ic;  return *this; }
rem& rem::operator << (Accent::Type ac) { _text << ac; return *this; }
rem& rem::operator << (color::type c)   { _text << c;  return *this; }
rem& rem::operator << (chattr::pair p)  { _text << p;  return *this; }
rem& rem::operator << (const point& p)  { _text << (std::string)p;   return *this; }
rem& rem::operator << (const dim& dd)   { _text << (std::string)dd;  return *this; }
rem& rem::operator << (const rect& dd)  { _text << (std::string)dd;  return *this; }






rem & rem::operator<<(rem::code c)
{

    switch(c)
    {
    case rem::begin:
        _indent = rem::indent();
        break;
    case rem::end:
        _indent = rem::unindent();
        break;
    case rem::endl:
    {
        //if(encode == chattr::format::ansi256)
        //{
            _text , '\n';
            //str_acc.fill(0x20, indent);
        //}
        //else
        //    if(encode == chattr::format::html)
        //        str_acc , "<br />";
        break;
    }
    case rem::stamp:
    {
        // Get the icon-color-pair from the application mem codes DB:
        auto [ic,p] = rem::attributes(rem::stamp);
        // Black ( p.bg == 0 ('\033[0m]') )
        _text , color::White , '[' , p.bg  , p.fg , Icon::Chronos , stracc::now("%X") , color::White , ']';
    }
    break;
    default:
    {
        // Get the icon-color-pair from the application mem codes DB:
        auto [ic,p] = rem::attributes(c);
        _text , color::White , '[' , p , Icon::Data[ic] , rem::text(c) , color::White , ']';
    }
    break;
    }

    return *this;
}




rem::code rem::init()
{
    return rem::accepted;
}


rem::attribute_data &rem::attributes(rem::code c)
{

    auto i = rem::codes_data.begin(), e =  rem::codes_data.end();
    for(;i !=e; i++)
        if(i->first == c)
            return i->second;

    return rem::codes_data.begin()->second;

}



rem::attribute_data &rem::attributes(rem::type t)
{
    auto i = rem::types_data.begin(), e =  rem::types_data.end();
    for(;i !=e; i++)
        if(i->first == t)
            return i->second;

    return rem::types_data.begin()->second;
}



rem& rem::push_error(source_location &&loc_)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::error,std::move(loc_));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_warning(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::warning,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_fatal(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::fatal,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_except(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::except,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_message(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::message,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::out(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::output,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_debug(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::debug,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_info(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::info,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_comment(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::comment,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_syntax(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::syntax,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_status(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::status,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_test(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::test,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_interrupted(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::interrupted,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_aborted(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::aborted,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}

rem& rem::push_segfault(source_location &&loc)
{
    guard.lock();
    if(!rem::mem_stream.empty())
        rem::mem_stream.back() << rem::endl;

    rem::mem_stream.emplace_back(rem::segfault,std::move(loc));
    guard.unlock();
    return rem::mem_stream.back();
}




std::string rem::cc()
{
    //guard.lock();
    auto [icon, p] = rem::attributes(ty);
    stracc str;
    stracc ind;
    ///@todo Setup indentation length ( in application config for ex. )
    if(_indent > 0)
    {
        //std::cout << "log_entry::indentation: " << a_entry.indent << '\n';
        ind.fill(0x20, _indent*4);
    }
    str += ind;
    if(ty != rem::output)
    {
        //Get the color pair from the application DB for the given rem::type:
        str , color::White , '[' , p ,  Icon::Data[icon] ,  rem::text(ty) , color::White , "] ";
    }

    if(loc)
    {
        if(!loc.filename.empty())
        {
            strbrk txt(std::string(loc.filename.data()),"/",false);
            strbrk::token_t::list l;
            auto cnt = txt(l,"/",false);
            ///@todo check cnt....

            str , color::White , '[';
            str , color::CadetBlue2 , Icon::Folder;
            if(cnt>1)
                str , ".../";

            str , l.back()() , color::White , ']';
        }
        if(loc.line > 0)
        {
            str , color::White , '|' , color::Lime , Icon::Baseline , loc.line , color::White , "|";
        }
        if(!loc.function_sig.empty())
        {
            str , color::SteelBlue1 , Icon::Function , loc.function_sig , color::White , ": ";
        }
        //str << (loge.encode == textattr::format::ansi256 ? "\n": "<br />") << ind;
    }

    // ...
    str , _text;
    //guard.unlock();
    return str();
}

void rem::clear(std::function<void(rem& le)> fn)
{
    for (auto& e : rem::mem_stream)
    {
        if(fn)
            fn(e);
        else
            std::cout << e.cc();
        e._text.clear();
    }
    rem::mem_stream.clear();
}


rem& rem::tail()
{
    return rem::mem_stream.back();
}

rem::code rem::assign_stream(std::fstream* /* cstream_ptr */)
{
    return rem::notimplemented;
}


}