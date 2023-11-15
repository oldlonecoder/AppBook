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
#include "AppBook/Util/StrAcc.h"
#include "AppBook/Book/BookEnums.h"


namespace Util {

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

    void SetId(const std::string& oid) { _Id = oid; }
    Object::Iterator GetChildIterator(Object* obj);
    Object::Iterator GetChildIteratorByID(const std::string& aid);


    //Book::Enums::Code Detach();
    Book::Enums::Code Detach(Object* ObjPtr=nullptr);
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

