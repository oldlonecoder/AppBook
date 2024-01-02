/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once

#include "AppBook/Book/AppBook.h"
#include "AppBook/Book/BookEnums.h"





namespace Book
{

/*!
 * \brief The Expect class
 *
 *
 * \code
 *  Expect<std::string&> foo() {
 *  // ...
 *  throw AppBook::Error() << Book::Result::Failed << " could not return the expected value...";
 *  or return *(new std::string("Hell-Oh! Cruel World"));
 *  if T is not a reference then there we can return AppBook::*...
 *  Using std::any has also its own hellish troubles struggling to remove ref casts and co. No miracle.
 */
template<typename T=Book::Enums::Code> class Expect
{
    AppBook::Section::Contents::Element* Fail{ nullptr };
    T value;
    
    bool good{ false };


public:
    Expect() = default;// { }
    Expect(AppBook::Section::Contents::Element& msg): Fail(&msg), good(false){}

    Expect(T val): value(val), good(true){}

    Expect(Expect&& e) noexcept {
        good = e.good;
        if(!good)
            Fail = std::move(e.fail);
        else
            value = std::move(e.value);
    }

    Expect(Expect& e){
        good = e.good;
        if(!good)
            Fail = e.fail;
        else
            value = e.value;
    }

    Expect(const Expect& e){
        good = e.good;
        if(!good)
            Fail = e.fail;
        else
            value = e.value;
    }

    ~Expect() =default;

    Expect& operator = (Expect&& e) noexcept{
        if(this==&e) return *this;
        good = e.good;
        if(!good)
            Fail = std::move(e.fail);
        else
            value = std::move(e.value);
        return *this;
    }

    Expect& operator =(const Expect& e){
        if(this == &e) return *this;
        good = e.good;
        if(!good)
            Fail = e.fail;
        else
            value = e.value;
        return *this;
    }

    T operator *(){
        if(!good)
            throw AppBook::Exception()
        [
            AppBook::Except() << Book::Result::Expected << " AppBook::Expect<...> good state, but is not."
        ];
        return value;
    }

    auto& operator()()
    {
        if(good)
            throw AppBook::Exception()[
                AppBook::Except() << Book::Result::Expected << " AppBook::Expect<...> non good state, but it is, use Expect<T>:: T operator *() instead."
        ];
        if(!Fail)
            throw AppBook::Exception()[
                AppBook::Except() << Book::Result::Expected << " AppBook::Expect<...>::fail non null, but it is."
            ];

        return *Fail;
    }


    /*!
     * \brief operator ->
     *
     * \note Advice: use with care! struct or class types. If not, then compile error...
     */
    auto operator ->(){
        if(!good)
            throw AppBook::Exception()[
                AppBook::Except() << Book::Result::Expected << " AppBook::Expect<...> good state, but is not."
            ];
        return value;
    }
    operator bool() { return good; }

};


}
