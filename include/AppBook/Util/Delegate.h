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

#include <AppBook/Book/BookEnums.h>
#include <functional>
#include <utility>

// ------- copied from, intellectual proprietary to:
// https://schneegans.github.io/tutorials/2015/09/20/signal-slot :
// 0xAC6A { 44138 }
// Modified and adapted for AppBook and basic threads, by oldlonecoder
// ------------------------------------------------------------------


template <typename... Args> class Delegate
{

public:
    std::string _id{ "anonymous signal" };
    using Slot = std::function<Book::Enums::Action(Args...)>;
    using Array = std::vector<typename Delegate::Slot>;
    using iterator = typename Delegate::Array::iterator;
    using Accumulator = std::vector<Book::Enums::Action>;
    Delegate() = default;
    ~Delegate() = default;
    explicit Delegate(std::string  id_) : _id(std::move(id_)) {}
    Delegate(std::string  id_, typename Delegate::Accumulator& acc) : _id(std::move(id_)), _acc(&acc) {}

    // Copy constructor and assignment create a new notifier.
    Delegate(Delegate const& /*unused*/) {}

    Delegate& operator=(Delegate const& other) {
        if (this != &other) {
            DisconnectAll();
        }
        return *this;
    }

    // Move constructor and assignment operator work as expected.
    Delegate(Delegate&& other) noexcept :
        _slots(std::move(other._slots)),
        _id(std::move(other._id)),
        _acc(std::move(other._acc))
    {}

    Delegate& operator=(Delegate&& other) noexcept {
        if (this != &other) {
            _slots = std::move(other._slots);
            _id = std::move(other._id);
            _acc = std::move(other._acc);
        }

        return *this;
    }


    // Connects a std::function to the notifier. The returned
    // value can be used to Disconnect the function again.
    typename Delegate::iterator Connect(std::function<Book::Enums::Action(Args...)> const& aslot) const {
        _slots.push_back(aslot);
        return --_slots.end();
    }

    // Convenience member method to Connect explicitely a member function of an
    // object to this notifier.
    template <typename T> typename Delegate::iterator ConnectMember(T* inst, Book::Enums::Action(T::* func)(Args...)) {
        return Connect([=](Args... args) {
            return (inst->*func)(args...);
        });
    }

    // Convenience method to Connect a member function of an
    // object to this notifier.
    template <typename T> typename Delegate::iterator Connect(T* inst, Book::Enums::Action(T::* func)(Args...)) {
        return Connect([=](Args... args) {
            return (inst->*func)(args...);
        });
    }


    // Convenience method to Connect a const member function
    // of an object to this notifier.
    template <typename T> typename Delegate::iterator Connect(T* inst, Book::Enums::Action(T::* func)(Args...) const) {
        return Connect([=](Args... args) {
            return (inst->*func)(args...);
        });
    }

    // Disconnects a previously connected function.
    void Disconnect(typename Delegate::iterator id) const {
        _slots.erase(id);
    }

    // Disconnects all previously connected functions.
    void DisconnectAll() const {
        _slots.clear();
    }

    //// Calls all connected functions.
    Book::Enums::Action operator()(Args... p) {
        if (_slots.empty())
            return Book::Enums::Action::Continue;
        Book::Enums::Action R;
        for (auto const&fn : _slots) {
            R = fn(p...);
            if (_acc) _acc->push_back(R);
            if (R != Book::Action::Continue) return R;
        }
        return R;
    }


    // Calls all connected functions except for one.
    Book::Enums::Action emit_for_all_but_one(const std::string& id_, Args... p) {
        Book::Enums::Action R;
        for (auto const& it : _slots) {
            if (it._id != id_) {
                R = it(p...);
                if (_acc) _acc->push_back(R);
                if (R != Book::Enums::Action::Continue) return R;
            }
        }
        return R;
    }

    // Calls only one connected function.
    Book::Enums::Action emit_for(typename Delegate::iterator id_, Args... p) {
        Book::Enums::Action R;
        if (id_ != _slots.end()) {
            R = (*id_)(p...);
            if (_acc) _acc->push_back(R);
            if (R != Book::Enums::Action::Continue) return R;
        }
        return R;
    }
    bool Empty() { return _slots.empty(); }
private:

    mutable typename Delegate::Array _slots;
    mutable typename Delegate::Accumulator* _acc{ nullptr };
};

template<typename Ret, typename ...Parameters> class Signal
{
    std::string _id{ "anonymous signal" };
public:
    using Slot = std::function<Ret(Parameters...)>;
    using Array = std::vector<typename Signal::Slot>;
    using Iterator = typename Signal::Array::iterator;
    using Accumulator = std::vector<Ret>;

    Signal() = default;
    ~Signal() = default;
    explicit Signal(std::string  id_) : _id(std::move(id_)) {}
    Signal(std::string  id_, typename Signal::Accumulator& acc) : _id(std::move(id_)), _acc(&acc) {}

    // Copy constructor and assignment create a new notifier.
    Signal(Signal const& /*unused*/) {}

    Signal& operator=(Signal const& other) {
        if (this != &other) {
            DisconnectAll();
        }
        return *this;
    }

    // Move constructor and assignment operator work as expected.
    Signal(Signal&& other) noexcept :
    _slots(std::move(other._slots)),
    _id(std::move(other._id)),
    _acc(std::move(other._acc))
    {}

    Signal& operator=(Signal&& other) noexcept {
        if (this != &other) {
            _slots = std::move(other._slots);
            _id = std::move(other._id);
            _acc = std::move(other._acc);
        }

        return *this;
    }


    // Connects a std::function to the notifier. The returned
    // value can be used to Disconnect the function again.
    typename Signal::Iterator Connect(std::function<Ret(Parameters...)> const& aslot) const {
        _slots.push_back(aslot);
        return --_slots.end();
    }

    // Convenience member method to Connect explicitly a member function of an
    // object to this notifier.
    template <typename T> typename Signal::Iterator ConnectMember(T* inst, Ret(T::* func)(Parameters...)) {
        return Connect([=](Parameters... Args) {
            return (inst->*func)(Args...);
        });
    }

    // Convenience method to Connect a member function of an
    // object to this notifier.
    template <typename T> typename Signal::Iterator Connect(T* inst, Ret(T::* func)(Parameters...)) {
        return Connect([=](Parameters... Args) {
            return (inst->*func)(Args...);
        });
    }


    // Convenience method to Connect a const member function
    // of an object to this notifier.
    template <typename T> typename Signal::Iterator Connect(T* inst, Ret(T::* func)(Parameters...) const) {
        return Connect([=](Parameters... Args) {
            return (inst->*func)(Args...);
        });
    }

    // Disconnects a previously connected function.
    void Disconnect(typename Signal::Iterator id) const {
        _slots.erase(id);
    }

    // Disconnects all previously connected functions.
    void DisconnectAll() const {
        _slots.clear();
    }

    //// Calls all connected functions.
    // std::pair<Book::Action, Ret>
    Ret operator()(Parameters... p) {
        if (_slots.empty())
            return Ret();
        Ret R;
        for (auto const&fn : _slots) {
            R = fn(p...);
            if (_acc) _acc->push_back(R);
        }
        return R;
    }


    // Calls all connected functions except for one.
    Ret emit_for_all_but_one(const std::string& id_, Parameters... p) {
        Ret R;
        for (auto const& it : _slots) {
            if (it._id != id_) {
                R = it(p...);
                if (_acc) _acc->push_back(R);
            }
        }
        return R;
    }

    // Calls only one connected function.
    Ret emit_for(typename Signal::Iterator id_, Parameters... p) {
        Ret R;
        if (id_ != _slots.end()) {
            R = (*id_)(p...);
            if (_acc) _acc->push_back(R);
        }
        return R;
    }
    bool Empty() { return _slots.empty(); }
private:

    mutable typename Signal::Array _slots;
    mutable typename Signal::Accumulator* _acc{ nullptr };
    
};