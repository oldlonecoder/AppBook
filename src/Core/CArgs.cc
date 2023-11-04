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


#include "AppBook/Core/CArgs.h"
#include <AppBook/Book/AppBook.h>



namespace Core::Cmd
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
        auto const* Str = Sv.data();
        auto NextArg = Query(Str);
        if(NextArg == Args.end())
        {
            if((CurArg != Args.end()) && ((*CurArg)->Required > (*CurArg)->Count) && ((*CurArg)->Required > 0))
            {
                (*CurArg)->Arguments.emplace_back(Str);
                ++(*CurArg)->Count;
                (*CurArg)->CanUse = true;
                AppBook::Out() << Core::Color::Yellow << (*CurArg)->Name << Core::Color::Reset << " Arg: '"<< Str;
            }
            else
            {
                Defaults.Arguments.emplace_back(Str);
                Defaults.CanUse = true;
            }
            continue;
        }
        else
            CurArg = NextArg;
    }

    return Book::Enums::Code::Ok;
}


/*!
     * \brief Initialize and bulds internal Data.
     * \param argc
     * \param argv
     * \return Status code
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
            if((CurArg != Args.end()) && ((*CurArg)->Required > (*CurArg)->Count) && ((*CurArg)->Required > 0))
            {
                (*CurArg)->Arguments.emplace_back(Str);
                ++(*CurArg)->Count;
                (*CurArg)->CanUse = true;
                AppBook::Out() << Core::Color::Yellow << (*CurArg)->Name << Core::Color::Reset << " Arg: '"<< Str;
            }
            else
            {
                Defaults.Arguments.emplace_back(Str);
                Defaults.CanUse = true;
            }
            continue;
        }
        else
            CurArg = NextArg;
    }


    return Book::Enums::Code::Ok;
}

Book::Enums::Code CArgs::Execute()
{
    auto R = Book::Enums::Code::Rejected;
    if(!Args.empty())
    {
        int b = 0;
        for(auto const& A : Args)if(A->CanUse) ++b;
        if(!b)
        {
            return Book::Enums::Code::Empty;
        }
        for(auto* Arg : Args)
        {
            if(!(*Arg)) continue; // No callback.
            if(Arg->CanUse)
            {
                R = Arg->DelegateCB(*Arg);
                if(R != Book::Enums::Code::Accepted)
                    return R;

            }
        }
    }
    if(!Defaults.Arguments.empty())
    {
        if(Defaults.CanUse)
        {
            R = Defaults.DelegateCB(Defaults);
            if(R != Book::Enums::Code::Accepted)
                return R;
        }
    }
    return Book::Enums::Code::Accepted;
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
    Str << "--------------------------------------------------------------------------\n";
    for (auto* Arg : Args)
    {
        Str << "%-2s | %-20s | %s" << Arg->SwitchChar << Arg->SwitchText << Arg->Description << '\n';
        Str << "--------------------------------------------------------------------------\n";
    }
    return Str();

}



ArgumentData::~ArgumentData()
{
    Arguments.clear();
    DelegateCB.DisconnectAll();
}
}



































