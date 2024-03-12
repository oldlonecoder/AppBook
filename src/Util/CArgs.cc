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


#include "AppBook/Util/CArgs.h"
#include <AppBook/Book/AppBook.h>



namespace Cmd
{


ArgumentData::Iterator CArgs::Query(const std::string &Switch)
{
    auto It = Args.begin();
    for (; It != Args.end(); It++)
    {
        if ((Switch == (*It)->SwitchText ) || (Switch == (*It)->SwitchChar))
            return It;
    }
    return Args.end();
}

CArgs::~CArgs()
{
    Args.clear(); // doh ...
}


/*!
 * @brief Input operator
 * @param Arg
 * @return Pointer to ArgumentData instance.
 * @author &copy;2023/2024, oldlonecoder/serge.lussier@oldlonecoder.club
 *
 * @note Because of the Delegate<>, We must instantiate a unique non-copiable ArgumentData using <i>new</i> to ensure
 * that there will be NO move/copy of the instances. We may instantiate directly into the Args using emplace_back tho. -
 * to be considered...
 */
ArgumentData &CArgs::operator<<(const ArgumentData &Arg)
{
    Args.push_back(new ArgumentData);
    ArgumentData* A = Args.back();
    A->Name = Arg.Name;
    A->SwitchChar = Arg.SwitchChar;
    A->SwitchText = Arg.SwitchText;
    A->Description = Arg.Description;
    A->Required = Arg.Required;

    return *A;
}

Book::Enums::Code CArgs::ProcessStringArray(std::vector<std::string_view> StrArray)
{
    auto CurArg = Args.end();
    for(auto Sv : StrArray)
    {
        auto NextArg = Query(Sv.data());


        if(NextArg == Args.end())
        {
            // It is not a switch - so must be an argument data for the CurArg/NextArg...
            if((CurArg != Args.end()) && ((*CurArg)->Required > (*CurArg)->Count) && ((*CurArg)->Required > 0))
            {
                (*CurArg)->Arguments.emplace_back(Sv);
                ++(*CurArg)->Count;
                (*CurArg)->Enabled = true;
                AppBook::Out() << Color::Yellow << (*CurArg)->Name << Color::Reset << " Arg: '" << Sv;
            }
            else
            {
                // ... or non-switch arg
                Defaults.Arguments.emplace_back(Sv);
                Defaults.Enabled = true;
            }
            continue;
        }
        else
        {
            // argv[i] is a switch,  then enable it....
            CurArg = NextArg;
            (*CurArg)->Enabled = true;
        }
    }

    return Book::Result::Ok;
}


/*!
     * \brief Initialize and bulds internal Data.
     * \param argc
     * \param argv
     * \return Status Code
     */
Book::Enums::Code CArgs::InputCmdLineData(int argc, char **argv)
{

    auto CurArg = Args.end();
    for(int i = 1; i < argc; i++)
    {
        auto const* Str = argv[i];
        auto NextArg = Query(Str);


        if(NextArg == Args.end())
        {
            // It is not a switch - so must be an argument data for the CurArg/NextArg...
            if((CurArg != Args.end()) && ((*CurArg)->Required > (*CurArg)->Count) && ((*CurArg)->Required > 0))
            {
                (*CurArg)->Arguments.emplace_back(Str);
                ++(*CurArg)->Count;
                (*CurArg)->Enabled = true;
                AppBook::Out() << Color::Yellow << (*CurArg)->Name << Color::Reset << " Arg: '" << Str;
            }
            else
            {
                // ... or non-switch arg
                Defaults.Arguments.emplace_back(Str);
                Defaults.Enabled = true;
            }
            continue;
        }
        else
        {
            // argv[i] is a switch,  then enable it....
            CurArg = NextArg;
            (*CurArg)->Enabled = true;
        }
    }

    return Book::Enums::Code::Ok;
}




Book::Action  CArgs::Execute()
{
    auto R = Book::Action::End;
    if(!Args.empty())
    {
        int b = 0;
        for(auto const& A : Args)if(A->Enabled) ++b;
        if(!b)
        {
            AppBook::Debug() << " No addressed arguments";
            return Book::Action::End;
        }
        for(auto* Arg : Args)
        {
            if(!(*Arg)) continue; // No callback.
            if(Arg->Enabled)
            {
                auto E = Arg->DelegateCB(*Arg);
                if(E != Book::Action::Continue)
                    return R;

            }
        }
    }
    if(!Defaults.Arguments.empty())
    {
        if(Defaults.Enabled)
        {
            auto R = Defaults.DelegateCB(Defaults);
            if(R != Book::Action::Continue)
                return R;
        }
    }
    return Book::Action::Continue;
}

ArgumentData &CArgs::operator[](const std::string &ArgName)
{
    for(auto* Arg :Args)
    {
        if(Arg->Name == ArgName) return *Arg;
    }
    return Defaults;
}

std::string CArgs::Usage()
{
    StrAcc Str;
    Str << "usage:\n";
    StrAcc Unused, Used, Used0;
    Unused = Used = "%s%s";
    Unused << Color::Red1 << Utf::Glyph::UnAuth;
    Used   << Color::LightGreen2 << Utf::Glyph::Ok;

    Str << "------------------------------------------------------------------------------\n";
    for (auto* Arg : Args)
    {
        Str << "%-2s | %-20s | %-45s %s" <<
            Arg->SwitchChar <<
            Arg->SwitchText <<
            Arg->Description <<
            (Arg->Enabled ? Used : Unused) <<
            Color::Reset << '\n';

        Str << "------------------------------------------------------------------------------\n";
    }
    return Str();

}



ArgumentData::~ArgumentData()
{
    Arguments.clear();
    DelegateCB.DisconnectAll();
}
}



































