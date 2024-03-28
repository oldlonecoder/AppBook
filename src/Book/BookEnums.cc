
#include <AppBook/Book/BookEnums.h>
#include <source_location>

#include <map>


bool operator !(Book::Enums::Code C){ return (C != Book::Result::Accepted) && (C != Book::Result::Ok) && (C != Book::Result::Success); }

namespace Book::Enums
{


std::map<Class, const char*> category_dictionary = {
    {Class::None,        "None"},
    {Class::Error,       "Error"},
    {Class::Warning,     "Warning"},
    {Class::Fatal,       "Fatal"},
    {Class::Except,      "Exception"},
    {Class::Message,     "Message"},
    {Class::Output,      ""},
    {Class::Debug,       "Debug"},
    {Class::Info,        "Info"},
    {Class::Comment,     "Comment"},
    {Class::Syntax,      "SyntaxError"},
    {Class::Status,      "Status"},
    {Class::Test,        "Test"},
    {Class::Interrupted, "Interrupted"},
    {Class::Aborted,     "Aborted"},
    {Class::Segfault,    "SegmentationFault"}
};


std::map<Code, const char*> message_codes_dictionary = {
    {Code::Ok,             "Ok"},
    {Code::Accepted,       "Accepted"},
    {Code::Success,        "Success "},
    {Code::Rejected,       "Rejected"},
    {Code::Failed,         "Failed"},
    {Code::Empty,          "Empty"},
    {Code::Full,           "Full"},
    {Code::Notempty,       "Notempty"},
    {Code::Implemented,    "Implemented"},
    {Code::Notimplemented, "Notimplemented"},
    {Code::Untested,       "Untested"},
    {Code::Eof,            "Eof"},
    {Code::Eos,            "Eos"},
    {Code::Null_ptr,       "Null_ptr"},
    {Code::Notexist,       "Notexist"},
    {Code::Exist,          "Exist"},
    {Code::Unexpected,     "Unexpected"},
    {Code::Expected,       "Expected"},
    {Code::Blocked,        "Blocked"},
    {Code::Locked,         "Locked"},
    {Code::Overflow,       "Buffer overflow"},
    {Code::Oob,            "Out of boundaries"},
    {Code::Reimplement,    "Need To be re-implemented IsIn derived instances"},
};


std::map<Action, const char*> actions_dictionary= {
    {Action::Enter, "Enter"},
    {Action::Leave, "Leave"},
    {Action::Begin, "Begin"},
    {Action::End,   "End"},
    {Action::Ci,    "Commit"},
    {Action::Continue, "Continue"},

};


std::map<Fn, const char*> Fn_dictionary= {

    {Fn::Endl,           "Endl"},
    {Fn::Function,       "Function"},
    {Fn::File,           "File"},
    {Fn::Line,           "Line"},
    {Fn::Stamp,          "Stamp"},
    {Fn::Hour,           "Hour"},
    {Fn::Minute,         "Minute"},
    {Fn::Seconds,        "Seconds"},
    {Fn::Weekday,        "Weekday"},
    {Fn::Monthname,      "Monthname"},
    {Fn::Dayname,        "Dayname"},
    {Fn::Day,            "Day"},
    {Fn::Month,          "Month"},
    {Fn::Year,           "Year"}
};

using namespace Utf;

std::map<Class, std::pair<Glyph::Type, Color::Pair>> categories_database={

    {Class::None,        {Glyph::Computer,  {Color::Aquamarine1         , Color::Reset }}},
    {Class::Error,       {Glyph::Err1,      {Color::Red4                , Color::Reset }}},
    {Class::Warning,     {Glyph::Warning,   {Color::Yellow              , Color::Reset }}},
    {Class::Fatal,       {Glyph::DeadHead,  {Color::HotPink4            , Color::Reset }}},
    {Class::Except,      {Glyph::Bolt,      {Color::SkyBlue3            , Color::Reset }}},
    {Class::Message,     {Glyph::Comment,   {Color::Green4              , Color::Reset }}},
    {Class::Output,      {Glyph::NotePad,   {Color::Grey39              , Color::Reset }}},
    {Class::Debug,       {Glyph::Bug,       {Color::Pink3               , Color::Reset }}},
    {Class::Info,        {Glyph::Info,      {Color::LightSkyBlue4       , Color::Reset }}},
    {Class::Comment,     {Glyph::Comment,   {Color::Grey42              , Color::Reset }}},
    {Class::Syntax,      {Glyph::Handwrite, {Color::LighcoreateBlue     , Color::Reset }}},
    {Class::Status,      {Glyph::Admin,     {Color::LightCyan3          , Color::Reset }}},
    {Class::Test,        {Glyph::File,      {Color::Aquamarine3         , Color::Reset }}},
    {Class::Interrupted, {Glyph::CircleX,   {Color::Khaki3              , Color::Reset }}},
    {Class::Aborted,     {Glyph::Stop,      {Color::Red4                , Color::Reset }}},
    {Class::Segfault,    {Glyph::Bomb,      {Color::SandyBrown          , Color::Reset }}}
    //...
};


std::map<Action, std::pair<Glyph::Type, Color::Pair>> ActionsColorDb={
    {Action::Enter, {Glyph::Enter, {Color::White, Color::Reset}}},
    {Action::Leave, {Glyph::File, {Color::White, Color::Reset}}},
    {Action::Begin, {Glyph::Chronos, {Color::White, Color::Reset}}},
    {Action::End,   {Glyph::DeadHead, {Color::White, Color::Reset}}},
    {Action::Ci,    {Glyph::PencilDr, {Color::White, Color::Reset}}},
    {Action::Continue, {Glyph::Success, {Color::White, Color::Reset}}},
};

std::map<Code, std::pair<Glyph::Type, Color::Pair>> codes_database={
    {Code::Ok,              {Glyph::ThumbUp,         {Color::DarkGreen         ,Color::Reset }}},
    {Code::Accepted,        {Glyph::CMarkCheck,      {Color::DarkGreen         ,Color::Reset }}},
    {Code::Success,         {Glyph::Success,         {Color::DarkGreen         ,Color::Reset }}},
    {Code::Rejected,        {Glyph::ThumbDown,       {Color::HotPink4          ,Color::Reset }}},
    {Code::Failed,          {Glyph::Poop,            {Color::DarkGoldenrod     ,Color::Reset }}},
    {Code::Empty,           {Glyph::ArrowRight,      {Color::LighcoreateGrey   ,Color::Reset }}},
    {Code::Full,            {Glyph::SmallDot,        {Color::White             ,Color::Reset }}},
    {Code::Notempty,        {Glyph::BigDot,          {Color::White             ,Color::Reset }}},
    {Code::Implemented,     {Glyph::ArrowHeadRight,  {Color::Lime              ,Color::Reset }}},
    {Code::Notimplemented,  {Glyph::Err1,            {Color::Orange3           ,Color::Reset }}},
    {Code::Untested,        {Glyph::Flag,            {Color::Yellow            ,Color::Reset }}},
    {Code::Eof,             {Glyph::Baseline,        {Color::White             ,Color::Reset }}},
    {Code::Eos,             {Glyph::Baseline,        {Color::White             ,Color::Reset }}},
    {Code::Null_ptr,        {Glyph::CircleX,         {Color::HotPink4          ,Color::Reset }}},
    {Code::Notexist  ,      {Glyph::CircleX ,        {Color::White             ,Color::Reset }}},
    {Code::Exist     ,      {Glyph::Star5 ,          {Color::White             ,Color::Reset }}},
    {Code::Unexpected,      {Glyph::Flag ,           {Color::Yellow            ,Color::Reset }}},
    {Code::Expected  ,      {Glyph::Cat ,            {Color::White             ,Color::Reset }}},
    {Code::Blocked   ,      {Glyph::UnAuth ,         {Color::IndianRed3        ,Color::Reset }}},
    {Code::Locked    ,      {Glyph::Err3 ,           {Color::White             ,Color::Reset }}},
    {Code::Overflow  ,      {Glyph::DeadHead,        {Color::Red4              ,Color::Reset }}},
    {Code::Oob       ,      {Glyph::Alien,           {Color::Red4              ,Color::Reset }}},
    {Code::Reimplement,{Glyph::Books,                {Color::Yellow            ,Color::Reset }}},

};


std::map<Fn, std::pair<Glyph::Type, Color::Pair>> Fn_database={

    {Fn::Stamp     ,      {Glyph::Chronos  ,   {Color::SkyBlue3          ,Color::Reset }}},
    {Fn::Function  ,      {Glyph::Function ,   {Color::SkyBlue3          ,Color::Reset }}},
    {Fn::File      ,      {Glyph::File     ,   {Color::White             ,Color::Reset }}},
    {Fn::Line      ,      {Glyph::Baseline ,   {Color::White             ,Color::Reset }}},
    {Fn::Stamp     ,      {Glyph::Chronos  ,   {Color::Yellow            ,Color::Reset }}},
    {Fn::Hour      ,      {Glyph::Chronos  ,   {Color::White             ,Color::Reset }}},
    {Fn::Minute    ,      {Glyph::Chronos  ,   {Color::White             ,Color::Reset }}},
    {Fn::Seconds   ,      {Glyph::Chronos  ,   {Color::White             ,Color::Reset }}},
    {Fn::Weekday   ,      {Glyph::Fabric   ,   {Color::Yellow            ,Color::Reset }}},
    {Fn::Monthname ,      {Glyph::Chronos  ,   {Color::White             ,Color::Reset }}},
    {Fn::Dayname   ,      {Glyph::Star5    ,   {Color::White             ,Color::Reset }}},
    {Fn::Day       ,      {Glyph::Star5    ,   {Color::White             ,Color::Reset }}},
    {Fn::Month     ,      {Glyph::Star5    ,   {Color::White             ,Color::Reset }}},
    {Fn::Year      ,      {Glyph::Star5    ,   {Color::White             ,Color::Reset }}}

};




std::string ClassText(Book::Enums::Class Cls)
{
    return category_dictionary[Cls];
}
std::string CodeText(Code c)
{
    return message_codes_dictionary[c];
}

std::pair<Glyph::Type, Color::Pair> ClassAttributes(Class Cls)
{
    return categories_database[Cls];
}

std::pair<Glyph::Type, Color::Pair> CodeAttributes(Code Cod)
{
    return codes_database[Cod];
}

std::pair<Glyph::Type, Color::Pair> FnAttributes(Fn f)
{
    return Fn_database[f];
}

std::pair<Utf::Glyph::Type, Color::Pair> ActionAttributes(Book::Enums::Action A)
{
    return ActionsColorDb[A];
}


}
