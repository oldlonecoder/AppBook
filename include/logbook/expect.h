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

#include "logbook/rem.h"



namespace book
{



/*!
 * \brief The expect class
 *
 *
 * \code
 *  expect<std::string&> foo() {
 *  // ...
 *  throw rem::push_error(HERE) << code::failed << " could not return the expected value...";
 *  or return *(new std::string("allo"));
 *  if T is not a reference then there we can return rem::push_*...
 *  Using std::any has also its own hellish troubles struggling to remove ref casts and co. No miracle.
 */
template<typename T=rem::code> class expect
{
    rem* fail{nullptr};
    T value;

    bool good = false;


public:
    expect() = default;// { }
    expect(rem& msg): fail(&msg),good(false){}

    expect(T val): value(val), good(true){}

    expect(expect&& e) noexcept {
        good = e.good;
        if(!good)
            fail = std::move(e.fail);
        else
            value = std::move(e.value);
    }

    expect(expect& e){
        good = e.good;
        if(!good)
            fail = e.fail;
        else
            value = e.value;
    }

    expect(const expect& e){
        good = e.good;
        if(!good)
            fail = e.fail;
        else
            value = e.value;
    }

    ~expect() =default;

    expect& operator = (expect&& e) noexcept{
        good = e.good;
        if(!good)
            fail = std::move(e.fail);
        else
            value = std::move(e.value);
        return *this;
    }

    expect& operator =(expect& e){
        good = e.good;
        if(!good)
            fail = e.fail;
        else
            value = e.value;
        return this;
    }

    expect& operator =(const expect& e){
        good = e.good;
        if(!good)
            fail = e.fail;
        else
            value = e.value;
        return this;
    }

    T operator *(){
        if(!good)
            throw rem::push_except() << rem::expected << " book::expect<...> good state, but is not.";
        return value;
    }

    auto& operator()()
    {
        if(good)
            throw rem::push_except() << rem::expected << " book::expect<...> non good state, but it is, use expect<T>:: T operator *() insbookd.";
        if(!fail)
            throw rem::push_except() << rem::expected << " book::expect<...>::fail non null, but it is.";

        return *fail;
    }


    /*!
     * \brief operator ->
     *
     * \note Advice: use with care! struct or class types. If not, then compile error...
     */
    auto operator ->(){
        if(!good)
            throw rem::push_except() << rem::expected << " book::expect<...> good state, but is not.";
        return value;
    }
    operator bool() { return good; }

};


}
