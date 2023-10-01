#include <logbook/book_data.h>






std::map<book::cat, const char*> category_dictionary = {
    {book::cat::none,             "none"},
    {book::cat::error,            "error"},
    {book::cat::warning,          "warning"},
    {book::cat::fatal,            "fatal"},
    {book::cat::except,           "expection"},
    {book::cat::message,          "message"},
    {book::cat::output,           ""},
    {book::cat::debug,            "debug"},
    {book::cat::info,             "information"},
    {book::cat::comment,          "comment"},
    {book::cat::syntax,           "syntax error"},
    {book::cat::status,           "status"},
    {book::cat::test,             "test"},
    {book::cat::interrupted,      "interrupted"},
    {book::cat::aborted,          "aborted"},
    {book::cat::segfault,         "segmentation fault"}
};


std::map<book::code, const char*> message_codes_dictionary = {
    {book::code::ok,             "ok"},
    {book::code::accepted,       "accepted"},
    {book::code::success,        "success "},
    {book::code::rejected,       "rejected"},
    {book::code::failed,         "failed"},
    {book::code::empty,          "empty"},
    {book::code::full,           "full"},
    {book::code::notempty,       "notempty"},
    {book::code::implemented,    "implemented"},
    {book::code::notimplemented, "notimplemented"},
    {book::code::untested,       "untested"},
    {book::code::eof,            "eof"},
    {book::code::eos,            "eos"},
    {book::code::null_ptr,       "null_ptr"},
    {book::code::notexist,       "notexist"},
    {book::code::exist,          "exist"},
    {book::code::unexpected,     "unexpected"},
    {book::code::expected,       "expected"},
    {book::code::blocked,        "blocked"},
    {book::code::locked,         "locked"},
    {book::code::overflow,       "buffer overflow"},
    {book::code::oob,            "out of boundaries"},
    {book::code::reimplement,"need to be re-implemented in derived instances"},
    };


std::map<book::action, const char*> actions_dictionary= {
    {book::action::enter,          "enter"},
    {book::action::leave,          "leave"},
    {book::action::begin,          "begin"},
    {book::action::end,            "end"},
    {book::action::ci,             "commit"},

    };


std::map<book::functions, const char*> functions_dictionary= {

    {book::functions::endl,           "endl"},
    {book::functions::function,       "function"},
    {book::functions::file,           "file"},
    {book::functions::line,           "line"},
    {book::functions::stamp,          "stamp"},
    {book::functions::hour,           "hour"},
    {book::functions::minute,         "minute"},
    {book::functions::seconds,        "seconds"},
    {book::functions::weekday,        "weekday"}


};


std::map<book::cat, std::pair<Icon::Type, std::pair<color::code, color::code>>> categories_database={

    {book::cat::none,        {Icon::Computer,    {color::Aquamarine1         ,color::Reset }}},
    {book::cat::error,       {Icon::Err1,        {color::Red4                ,color::Reset }}},
    {book::cat::warning,     {Icon::Warning,     {color::Yellow              ,color::Reset }}},
    {book::cat::fatal,       {Icon::DeadHead,    {color::HotPink4            ,color::Reset }}},
    {book::cat::except,      {Icon::Bolt,        {color::SkyBlue3            ,color::Reset }}},
    {book::cat::message,     {Icon::Comment,     {color::Green4              ,color::Reset }}},
    {book::cat::output,      {Icon::NotePad,     {color::Grey39              ,color::Reset }}},
    {book::cat::debug,       {Icon::Bug,         {color::Pink3               ,color::Reset }}},
    {book::cat::info,        {Icon::Info,        {color::LightSkyBlue4       ,color::Reset }}},
    {book::cat::comment,     {Icon::Comment,     {color::Grey42              ,color::Reset }}},
    {book::cat::syntax,      {Icon::Handwrite,   {color::LighcoreateBlue     ,color::Reset }}},
    {book::cat::status,      {Icon::Admin,       {color::LightCyan3          ,color::Reset }}},
    {book::cat::test,        {Icon::File,        {color::Aquamarine3         ,color::Reset }}},
    {book::cat::interrupted, {Icon::CircleX,     {color::Khaki3              ,color::Reset }}},
    {book::cat::aborted,     {Icon::Stop,        {color::Red4                ,color::Reset }}},
    {book::cat::segfault,    {Icon::Bomb,        {color::SandyBrown          ,color::Reset }}}
    //...
};


std::map<book::code, std::pair<Icon::Type, std::pair<color::code, color::code>>> codes_database={
    {book::code::ok,              {Icon::ThumbUp,         {color::DarkGreen         ,color::Reset }}},
    {book::code::accepted,        {Icon::Success,         {color::DarkGreen         ,color::Reset }}},
    {book::code::success,         {Icon::Success,         {color::DarkGreen         ,color::Reset }}},
    {book::code::rejected,        {Icon::ThumbDown,       {color::Orange5           ,color::Reset }}},
    {book::code::failed,          {Icon::Poop,            {color::DarkGoldenrod     ,color::Reset }}},
    {book::code::empty,           {Icon::ArrowRight,      {color::LighcoreateGrey   ,color::Reset }}},
    {book::code::full,            {Icon::SmallDot,        {color::White             ,color::Reset }}},
    {book::code::notempty,        {Icon::BigDot,          {color::White             ,color::Reset }}},
    {book::code::implemented,     {Icon::ArrowHeadRight,  {color::Lime              ,color::Reset }}},
    {book::code::notimplemented,  {Icon::Err1,            {color::Orange3           ,color::Reset }}},
    {book::code::untested,        {Icon::Flag,            {color::Yellow            ,color::Reset }}},
    {book::code::eof,             {Icon::Baseline,        {color::White             ,color::Reset }}},
    {book::code::eos,             {Icon::Baseline,        {color::White             ,color::Reset }}},
    {book::code::null_ptr,        {Icon::CircleX,         {color::HotPink4          ,color::Reset }}},
    {book::code::notexist  ,      {Icon::CircleX ,        {color::White             ,color::Reset }}},
    {book::code::exist     ,      {Icon::Star5 ,          {color::White             ,color::Reset }}},
    {book::code::unexpected,      {Icon::Flag ,           {color::Yellow            ,color::Reset }}},
    {book::code::expected  ,      {Icon::Cat ,            {color::White             ,color::Reset }}},
    {book::code::blocked   ,      {Icon::UnAuth ,         {color::IndianRed3        ,color::Reset }}},
    {book::code::locked    ,      {Icon::Err3 ,           {color::White             ,color::Reset }}},
    {book::code::overflow  ,      {Icon::DeadHead,        {color::Red4              ,color::Reset }}},
    {book::code::oob       ,      {Icon::Alien,           {color::Red4              ,color::Reset }}},
    {book::code::reimplement,{Icon::Books,                {color::Yellow            ,color::Reset }}},

    };

std::map<book::functions, std::pair<Icon::Type, std::pair<color::code, color::code>>> functions_database={

    {book::functions::stamp     ,      {Icon::Chronos ,        {color::SkyBlue3          ,color::Reset }}},
    {book::functions::function  ,      {Icon::Function ,       {color::SkyBlue3          ,color::Reset }}},
    {book::functions::file      ,      {Icon::File ,           {color::White             ,color::Reset }}},
    {book::functions::line      ,      {Icon::Baseline ,       {color::White             ,color::Reset }}},
    {book::functions::stamp     ,      {Icon::Chronos ,        {color::Yellow            ,color::Reset }}},
    {book::functions::hour      ,      {Icon::Chronos ,        {color::White             ,color::Reset }}},
    {book::functions::minute    ,      {Icon::Chronos ,        {color::White             ,color::Reset }}},
    {book::functions::seconds   ,      {Icon::Chronos ,        {color::White             ,color::Reset }}},
    {book::functions::weekday   ,      {Icon::Chronos ,        {color::White             ,color::Reset }}}
};




std::string category_text(book::cat c)
{
    return category_dictionary[c];
}
