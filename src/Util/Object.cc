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

#include "AppBook/Util/Object.h"

namespace Util{

Object::Object(Object *aparent, const std::string &aobj_name):
    _Children({}), _Id(aobj_name), _Parent(aparent)
{
    if(_Parent)
        _Parent->AppendChild(this);
}

Object::~Object()
{
    //Detach();
    _Children.clear();
}

Object::Iterator Object::GetChildIterator(Object *obj)
{
    if(_Children.empty()) return _Children.end();
    auto ot = _Children.begin();
    while(ot != _Children.end())
    {
        if(*ot == obj) return ot;
        ++ot;
    }
    return _Children.end();
}

[[maybe_unused]] Object::Iterator Object::GetChildIteratorByID(const std::string &aid)
{
    //std::cerr << __PRETTY_FUNCTION__ << " - " << Id() << "::" << aid << ":\n";
    if(_Children.empty())
    {
        std::cerr << " .. no children...\n";
        return _Children.end();
    }
    auto ot = _Children.begin();
    while(ot != _Children.end())
    {
        if((*ot)->_Id == aid) return ot;
        ot++;
    }
    std::cerr << Color::Ansi(Color::White) << Id() << " has no child identified by '"
              << Color::Ansi(Color::Yellow) << aid
              << Color::Ansi(Color::White) << "'\n";

    return _Children.end();
}

Object::Object(Object &&obj) noexcept : _Children(std::move(obj._Children)), _Id(std::move(obj._Id)), _Parent(obj._Parent) {}

Object::Object(const Object &obj): _Children(obj._Children), _Id(obj._Id), _Parent(obj._Parent){}

void Object::AppendChild(Object *o)
{
    if(GetChildIterator(o) != _Children.end()) return;
    _Children.push_back(o);
    o->_Parent = this;
}

Book::Enums::Code Object::Detach(Object *ObjPtr)
{
    if(ObjPtr)
    {
        auto O = GetChildIterator(ObjPtr);
        if(O == _Children.end() ) return Book::Enums::Code::Rejected;
        _Children.erase(O);
        return Book::Enums::Code::Accepted;
    }

    if(!_Parent) return Book::Enums::Code::Rejected;
    _Parent->Detach(this);
    return Book::Enums::Code::Accepted;
}


} // oo
