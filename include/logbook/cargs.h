#pragma once

#include <logbook/notify.h>



namespace book::cmd
{


template <typename T> struct BOOK_PUBLIC argdata
{
    using data = std::vector<std::string_view>;
    using list = std::vector<argdata<T>>;
    using iterator = argdata::list::iterator;
    using callback = rem::code(T::*)(const argdata<T>&);
    using type = T*;
    T*                     obj{nullptr};
    callback               cb{nullptr};
    std::string            name;
    std::string            sw_char;
    std::string            sw_text;
    std::string            description;
    int                    required { 0 };
    int                    count    { 0 };

    //notify<const argdata&> callback;
    data                   arguments{};
    bool                   uses{ false };

    bool operator !() { return (!obj) || (!uses) || (!cb); }

    void connect(callback acb) { cb = acb; }
    ~argdata() { arguments.clear(); }
};


template<typename T> class BOOK_PUBLIC cargs
{
    T* obj{nullptr};
    argdata<T>::list args;
    argdata<T> defaults{ obj, nullptr, "defaults", "","", "default(orphan) arguments that have no switch",65000,0};

public:
    cargs() = default;
    ~cargs() { args.clear(); }

    rem::code set_default_callback(argdata<T>::callback acb)
    {
        defaults.cb = acb;
        return rem::ok;
    }


    argdata<T>::iterator query(const std::string& sw)
    {
        for (auto i = args.begin(); i != args.end(); i++)
        {
            if (sw == i->sw_text) return i;
            if (sw == i->sw_char) return i;
        }
        return args.end();
    }


    argdata<T>& operator<<(const argdata<T>& a)
    {
        if(!obj)
            obj = a.obj; ///< only needed once.

        args.push_back(a);
        return args.back();
    }

    /*!
 * \brief cargs::process
 * \param argc
 * \param argv
 * \return book::rem::code
 */
    rem::code process(int argc, char** argv)
    {
        typename argdata<T>::iterator arg = args.end();
        for (int i = 1; i < argc; i++)
        {
            auto const* carg = argv[i];
            auto next_arg = query(carg);
            if(next_arg == args.end())
            {
                if ((arg != args.end()) && (arg->required > arg->count) && (arg->required > 0)) // first pass it is the same as null_arg then use the defaults args)
                {
                    rem::push_debug(HERE) << " adding argument '" << color::Yellow << carg << color::Reset << "' to '" << color::LighcoreateBlue << arg->name << '\'' << rem::commit;
                    arg->arguments.push_back(carg);
                    ++arg->count;
                    arg->uses = true;
                }
                else
                {
                    rem::push_debug(HERE) << " adding argument '" << color::Yellow << carg << color::Reset << "' to '" << color::LighcoreateBlue << "defaults"  << '\'' << rem::commit;
                    defaults.arguments.push_back(carg);
                    defaults.uses = true;
                }
                continue;
            }
            else
                arg = next_arg;
        }

        rem::push_debug(HERE) << " Executing callbacks:" << rem::commit;
        rem::code R{rem::rejected};
        if(!args.empty())
        {
            int b = 0;
            for(auto const& a : args ) if(a.uses) ++b;
            if(!b)
            {
                rem::push_error() << "Expected command-line arguments :" << rem::endl << usage() << rem::commit;
                return R;
            }
        }
        for(auto& arg: args)
        {
            if(!arg) continue;
            rem::push_debug() << "arg '" << color::Yellow << arg.name << color::Reset << "' callback use:" << color::LighcoreateBlue << (arg.uses? " set " : " not set ") << rem::commit;
            if(arg.uses)
            {
                R = (obj->*arg.cb)(arg);
                if(R != rem::accepted) return R; // break the loop at any error.
            }
        }

        rem::push_debug(HERE) << " Executing defaults callbacks:" << rem::commit;
        if(defaults.uses)
            R = (obj->*defaults.cb)(defaults);

        return R;
    }


    std::string usage()
    {
        stracc str;
        str << "usage:\n";
        str << "--------------------------------------------------------------------------\n";
        for (auto const& arg : args)
        {
            str << "%-2s | %-20s | %s" << arg.sw_char << arg.sw_text << arg.description << "\n";
            str << "--------------------------------------------------------------------------\n";
        }
        return str();

    }

};

}
