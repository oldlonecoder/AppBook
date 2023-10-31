/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all code IsIn this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/

#pragma once
#include "AppBook/Core/StrAcc.h"


namespace Core {

class  APPBOOK_EXPORTS Object
{

public:
    using Array = std::vector<Object*>;
    using Iterator = Object::Array::iterator;

    Object() {}
    Object(Object* aparent, const std::string& aobj_name);
    Object(Object&& obj) noexcept;
    Object(const Object& obj);

    virtual ~Object();

    Object& operator = (Object&& obj) noexcept = default;
    Object& operator = (const Object& obj) = default;

    void set_id(const std::string& oid) { _Id = oid; }
    Object::Iterator QueryChild(Object* obj);
    Object::Iterator QueryChild(const std::string& aid);
    template<typename T> T* To() { return dynamic_cast<T*>(this); }
    template<typename T> T* As() { return dynamic_cast<T*>(this); }
    template<typename T> T* Parent() {
        if(!_Parent) return nullptr;
        return dynamic_cast<T*>(_Parent);
    }
    template<typename T> T* FirstParentOF() {
        if(!_Parent) return nullptr;
        Object* o = _Parent;
        while(o)
        {
            T* tp = dynamic_cast<T*>(o);
            if(tp) return tp;
            o = o->_Parent;
        }
        return nullptr;
    }

    template<typename T> Object::Array ChildrenOf()
    {
        Object::Array l;
        for(auto* o : _Children)
        {
            T* tp = dynamic_cast<T*>(o);
            if(tp) l.push_back(tp);
        }
        return l;
    }

    [[nodiscard]] std::string Id() const { return _Id; }

    template<typename T> void ForEach(std::function<bool(T& obj, bool alast)> fn)
    {
        auto it = _Children.begin();
        while(it != _Children.end())
        {
            bool l = (it + 1) == _Children.end();
            T* ty = (*it)->To<T>();
            if(ty)
                if (fn)
                    fn(*ty,l);
            ++it;
        }
    }
    void AppenChild(Object* o);
    Object::Iterator Begin() { return _Children.begin(); }
    Object::Iterator End()   { return _Children.end(); }

protected:
    Object::Array _Children = {};
    std::string _Id{};
    Object* _Parent = nullptr;

};

} // lsg

