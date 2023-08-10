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

#include "logbook/object.h"

namespace book{

object::object(object *aparent, const std::string &aobj_name):
 _children({}),_id(aobj_name),_parent(aparent)
{
    if(_parent)
        _parent->append_child(this);
}

object::~object()
{

    _children.clear();
}

object::iterator object::query_child(object *obj)
{
    if(_children.empty()) return _children.end();
    auto ot = _children.begin();
    while(ot != _children.end())
    {
        if(*ot == obj) return ot;
        ++ot;
    }
    return _children.end();
}

object::iterator object::query_child(const std::string &aid)
{
    //std::cerr << __PRETTY_FUNCTION__ << " - " << id() << "::" << aid << ":\n";
    if(_children.empty())
    {
        std::cerr << " .. no children...\n";
        return _children.end();
    }
    auto ot = _children.begin();
    while(ot != _children.end())
    {
        if((*ot)->_id == aid) return ot;
        ot++;
    }
    std::cerr << attr<chattr::format::ansi256>::fg(color::White) << id() << " has no child identified by '"
              << attr<chattr::format::ansi256>::fg(color::Yellow) << aid
              <<attr<chattr::format::ansi256>::fg(color::White) << "'\n";

    return _children.end();
}

object::object(object &&obj): _children(std::move(obj._children)), _id(std::move(obj._id)), _parent(obj._parent) {}

object::object(const object &obj):_children(obj._children),  _id(obj._id),_parent(obj._parent){}

void object::append_child(object *o)
{
    if(query_child(o) != _children.end()) return;
    _children.push_back(o);
    o->_parent = this;
}


} // oo
