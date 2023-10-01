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
#include "chrtools/stracc.h"
#include <logbook/public.h>

namespace book {

class  BOOK_PUBLIC object
{

public:
    using list = std::vector<object*>;
    using iterator = object::list::iterator;

    object() {}
    object(object* aparent, const std::string& aobj_name);
    object(object&& obj);
    object(const object& obj);

    virtual ~object();

    object& operator = (object&& obj) noexcept = default;
    object& operator = (const object& obj) = default;

    void set_id(const std::string& oid) { _id = oid; }
    object::iterator query_child(object* obj);
    object::iterator query_child(const std::string& aid);
    template<typename T> T* to() { return dynamic_cast<T*>(this); }
    template<typename T> T* as() { return dynamic_cast<T*>(this); }
    template<typename T> T* parent() {
        if(!_parent) return nullptr;
        return dynamic_cast<T*>(_parent);
    }
    template<typename T> T* first_parent_of() {
        if(!_parent) return nullptr;
        object* o = _parent;
        while(o)
        {
            T* tp = dynamic_cast<T*>(o);
            if(tp) return tp;
            o = o->_parent;
        }
        return nullptr;
    }

    template<typename T> object::list children_of()
    {
        object::list l;
        for(auto* o : _children)
        {
            T* tp = dynamic_cast<T*>(o);
            if(tp) l.push_back(tp);
        }
        return l;
    }

    [[nodiscard]] std::string id() const { return _id; }

    template<typename T> void for_each(std::function<bool(T& obj, bool alast)> fn)
    {
        auto it = _children.begin();
        while(it != _children.end())
        {
            bool l = (it + 1) == _children.end();
            T* ty = (*it)->to<T>();
            if(ty)
                if (fn)
                    fn(*ty,l);
            ++it;
        }
    }
    void append_child(object* o);
    object::iterator begin() { return _children.begin(); }
    object::iterator end()   { return _children.end(); }

protected:
    object::list _children = {};
    std::string _id{};
    object* _parent = nullptr;

};

} // lsg

