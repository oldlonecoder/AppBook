#include <logbook/book_data.h>




namespace book
{


std::map<cat, const char*> category_dictionary = {
    {cat::none,             "none"},
    {cat::error,            "error"},
    {cat::warning,          "warning"},
    {cat::fatal,            "fatal"},
    {cat::except,           "expection"},
    {cat::message,          "message"},
    {cat::output,           ""},
    {cat::debug,            "debug"},
    {cat::info,             "information"},
    {cat::comment,          "comment"},
    {cat::syntax,           "syntax error"},
    {cat::status,           "status"},
    {cat::test,             "test"},
    {cat::interrupted,      "interrupted"},
    {cat::aborted,          "aborted"},
    {cat::segfault,         "segmentation fault"}
};


std::map<code, const char*> message_codes_dictionary = {
    {code::ok,             "ok"},
    {code::accepted,       "accepted"},
    {code::success,        "success "},
    {code::rejected,       "rejected"},
    {code::failed,         "failed"},
    {code::empty,          "empty"},
    {code::full,           "full"},
    {code::notempty,       "notempty"},
    {code::implemented,    "implemented"},
    {code::notimplemented, "notimplemented"},
    {code::untested,       "untested"},
    {code::eof,            "eof"},
    {code::eos,            "eos"},
    {code::null_ptr,       "null_ptr"},
    {code::notexist,       "notexist"},
    {code::exist,          "exist"},
    {code::unexpected,     "unexpected"},
    {code::expected,       "expected"},
    {code::blocked,        "blocked"},
    {code::locked,         "locked"},
    {code::overflow,       "buffer overflow"},
    {code::oob,            "out of boundaries"},
    {code::reimplement,"need to be re-implemented in derived instances"},
    };


std::map<action, const char*> actions_dictionary= {
    {action::enter,          "enter"},
    {action::leave,          "leave"},
    {action::begin,          "begin"},
    {action::end,            "end"},
    {action::ci,             "commit"},

    };


std::map<functions, const char*> functions_dictionary= {

    {functions::endl,           "endl"},
    {functions::function,       "function"},
    {functions::file,           "file"},
    {functions::line,           "line"},
    {functions::stamp,          "stamp"},
    {functions::hour,           "hour"},
    {functions::minute,         "minute"},
    {functions::seconds,        "seconds"},
    {functions::weekday,        "weekday"},
    {functions::monthname,      "monthname"},
    {functions::dayname,        "dayname"},
    {functions::day,            "day"},
    {functions::month,          "month"},
    {functions::year,           "year"}
};


std::map<cat, std::pair<Icon::Type, chattr::pair>> categories_database={

    {cat::none,        {Icon::Computer,    {color::Aquamarine1         ,color::Reset }}},
    {cat::error,       {Icon::Err1,        {color::Red4                ,color::Reset }}},
    {cat::warning,     {Icon::Warning,     {color::Yellow              ,color::Reset }}},
    {cat::fatal,       {Icon::DeadHead,    {color::HotPink4            ,color::Reset }}},
    {cat::except,      {Icon::Bolt,        {color::SkyBlue3            ,color::Reset }}},
    {cat::message,     {Icon::Comment,     {color::Green4              ,color::Reset }}},
    {cat::output,      {Icon::NotePad,     {color::Grey39              ,color::Reset }}},
    {cat::debug,       {Icon::Bug,         {color::Pink3               ,color::Reset }}},
    {cat::info,        {Icon::Info,        {color::LightSkyBlue4       ,color::Reset }}},
    {cat::comment,     {Icon::Comment,     {color::Grey42              ,color::Reset }}},
    {cat::syntax,      {Icon::Handwrite,   {color::LighcoreateBlue     ,color::Reset }}},
    {cat::status,      {Icon::Admin,       {color::LightCyan3          ,color::Reset }}},
    {cat::test,        {Icon::File,        {color::Aquamarine3         ,color::Reset }}},
    {cat::interrupted, {Icon::CircleX,     {color::Khaki3              ,color::Reset }}},
    {cat::aborted,     {Icon::Stop,        {color::Red4                ,color::Reset }}},
    {cat::segfault,    {Icon::Bomb,        {color::SandyBrown          ,color::Reset }}}
    //...
};




std::map<code, std::pair<Icon::Type, chattr::pair>> codes_database={
    {code::ok,              {Icon::ThumbUp,         {color::DarkGreen         ,color::Reset }}},
    {code::accepted,        {Icon::CMarkCheck,      {color::DarkGreen         ,color::Reset }}},
    {code::success,         {Icon::Success,         {color::DarkGreen         ,color::Reset }}},
    {code::rejected,        {Icon::ThumbDown,       {color::Orange5           ,color::Reset }}},
    {code::failed,          {Icon::Poop,            {color::DarkGoldenrod     ,color::Reset }}},
    {code::empty,           {Icon::ArrowRight,      {color::LighcoreateGrey   ,color::Reset }}},
    {code::full,            {Icon::SmallDot,        {color::White             ,color::Reset }}},
    {code::notempty,        {Icon::BigDot,          {color::White             ,color::Reset }}},
    {code::implemented,     {Icon::ArrowHeadRight,  {color::Lime              ,color::Reset }}},
    {code::notimplemented,  {Icon::Err1,            {color::Orange3           ,color::Reset }}},
    {code::untested,        {Icon::Flag,            {color::Yellow            ,color::Reset }}},
    {code::eof,             {Icon::Baseline,        {color::White             ,color::Reset }}},
    {code::eos,             {Icon::Baseline,        {color::White             ,color::Reset }}},
    {code::null_ptr,        {Icon::CircleX,         {color::HotPink4          ,color::Reset }}},
    {code::notexist  ,      {Icon::CircleX ,        {color::White             ,color::Reset }}},
    {code::exist     ,      {Icon::Star5 ,          {color::White             ,color::Reset }}},
    {code::unexpected,      {Icon::Flag ,           {color::Yellow            ,color::Reset }}},
    {code::expected  ,      {Icon::Cat ,            {color::White             ,color::Reset }}},
    {code::blocked   ,      {Icon::UnAuth ,         {color::IndianRed3        ,color::Reset }}},
    {code::locked    ,      {Icon::Err3 ,           {color::White             ,color::Reset }}},
    {code::overflow  ,      {Icon::DeadHead,        {color::Red4              ,color::Reset }}},
    {code::oob       ,      {Icon::Alien,           {color::Red4              ,color::Reset }}},
    {code::reimplement,{Icon::Books,                {color::Yellow            ,color::Reset }}},

};





std::map<functions, std::pair<Icon::Type, chattr::pair>> functions_database={

    {functions::stamp     ,      {Icon::Chronos  ,   {color::SkyBlue3          ,color::Reset }}},
    {functions::function  ,      {Icon::Function ,   {color::SkyBlue3          ,color::Reset }}},
    {functions::file      ,      {Icon::File     ,   {color::White             ,color::Reset }}},
    {functions::line      ,      {Icon::Baseline ,   {color::White             ,color::Reset }}},
    {functions::stamp     ,      {Icon::Chronos  ,   {color::Yellow            ,color::Reset }}},
    {functions::hour      ,      {Icon::Chronos  ,   {color::White             ,color::Reset }}},
    {functions::minute    ,      {Icon::Chronos  ,   {color::White             ,color::Reset }}},
    {functions::seconds   ,      {Icon::Chronos  ,   {color::White             ,color::Reset }}},
    {functions::weekday   ,      {Icon::Fabric   ,   {color::Yellow            ,color::Reset }}},
    {functions::monthname ,      {Icon::Chronos  ,   {color::White             ,color::Reset }}},
    {functions::dayname   ,      {Icon::Star5    ,   {color::White             ,color::Reset }}},
    {functions::day       ,      {Icon::Star5    ,   {color::White             ,color::Reset }}},
    {functions::month     ,      {Icon::Star5    ,   {color::White             ,color::Reset }}},
    {functions::year      ,      {Icon::Star5    ,   {color::White             ,color::Reset }}}

};




std::string category_text(cat c)
{
    return category_dictionary[c];
}
std::string code_text(code c)
{
    return message_codes_dictionary[c];
}

std::pair<Icon::Type, chattr::pair> category_attributes(cat cat)
{
    return categories_database[cat];
}

std::pair<Icon::Type, chattr::pair> code_attributes(code c)
{
    return codes_database[c];
}

std::pair<Icon::Type, chattr::pair> function_attributes(functions f)
{
    return functions_database[f];
}



}
