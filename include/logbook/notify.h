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

#include "logbook/expect.h"
#include <functional>

// ------- copied from, intellectual proprietary to:
// https://schneegans.github.io/tutorials/2015/09/20/signal-slot :

// Modified and adapted for my logbook library and for using basic threads, by oldlonecoder
// ------------------------------------------------------------------

namespace book
{


template <typename... Args> class notify {

public:
    std::string _id{ "anonymous signal" };
    using slot = std::function<expect<>(Args...)>;
    using list = std::vector<typename notify::slot>;
    using iterator = typename notify::list::iterator;
    using accumulator = std::vector<expect<>>;
    notify() = default;
    ~notify() = default;
    notify(const std::string& id_) : _id(id_) {}
    notify(const std::string& id_, typename notify::accumulator& acc) : _id(id_), _acc(&acc) {}

    // Copy constructor and assignment create a new notifier.
    notify(notify const& /*unused*/) {}

    notify& operator=(notify const& other) {
        if (this != &other) {
            _slots = other._slots;
        }
        return *this;
    }

    // Move constructor and assignment operator work as expected.
    notify(notify&& other) noexcept :
        _slots(std::move(other._slots)),
        _id(std::move(other._id)),
        _acc(std::move(other._acc))
    {}

    notify& operator=(notify&& other) noexcept {
        if (this != &other) {
            _slots = std::move(other._slots);
            _id = std::move(other._id);
            _acc = std::move(other._acc);
        }

        return *this;
    }


    // Connects a std::function to the notifier. The returned
    // value can be used to disconnect the function again.
    typename notify::iterator connect(std::function<expect<>(Args...)> const& aslot) const {
        _slots.push_back(aslot);
        return --_slots.end();
    }

    // Convenience member method to connect explicitely a member function of an
    // object to this notifier.
    template <typename T> typename notify::iterator connect_member(T* inst, expect<>(T::* func)(Args...)) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
            });
    }

    // Convenience method to connect a member function of an
    // object to this notifier.
    template <typename T> typename notify::iterator connect(T* inst, expect<>(T::* func)(Args...)) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
            });
    }


    // Convenience method to connect a const member function
    // of an object to this notifier.
    template <typename T> typename notify::iterator connect(T* inst, expect<>(T::* func)(Args...) const) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
            });
    }

    // Disconnects a previously connected function.
    void disconnect(typename notify::iterator id) const {
        _slots.erase(id);
    }

    // Disconnects all previously connected functions.
    void disconnect_all() const {
        _slots.clear();
    }

    //// Calls all connected functions.
    expect<> operator()(Args... p) {
        if (_slots.empty())
        {
            rem::push_warning(HERE) << " no notify slot." << rem::commit;
            return rem::notexist;
        }
        expect<> R;
        for (auto const&fn : _slots) {
            R = fn(p...);
            if (_acc) _acc->push_back(R);
            if (!R || *R == rem::rejected) return R;
        }
        return R;
    }


    // Calls all connected functions except for one.
    expect<> emit_for_all_but_one(const std::string& id_, Args... p) {
        expect <> R;
        for (auto const& it : _slots) {
            if (it._id != id_) {
                R = it(p...);
                if (_acc) _acc->push_back(R);
                if (!R || *R == rem::rejected) return R;
            }
        }
        return R;
    }

    // Calls only one connected function.
    expect<> emit_for(typename notify::iterator id_, Args... p) {
        expect<> R;
        if (id_ != _slots.end()) {
            R = (*id_)(p...);
            if (_acc) _acc->push_back(R);
            if (!R || *R == rem::rejected) return R;
        }
        return R;
    }
    bool empty() { return _slots.empty(); }
private:

    mutable typename notify::list _slots;
    mutable typename notify::accumulator* _acc{ nullptr };
};


}
