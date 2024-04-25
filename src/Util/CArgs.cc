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


Switch::Iterator CArgs::Query(std::string_view SwitchData)
{
    auto It = Args.begin();
    for (; It != Args.end(); It++)
    {
        if ((SwitchData == (*It)->SwitchText ) || (SwitchData == (*It)->SwitchChar))
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
 * @return Pointer to Switch instance.
 * @author &copy;2023/2024, oldlonecoder/serge.lussier@oldlonecoder.club
 *
 * @note Because of the Delegate<>, We must instantiate a unique non-copiable Switch using <i>new</i> to ensure
 * that there will be NO move/copy of the instances. We may instantiate directly into the Args using emplace_back tho. -
 * to be considered...
 */
Switch &CArgs::operator<<(const Switch &Arg)
{
    Args.push_back(new Switch);
    Switch* Sw = Args.back();
    Sw->Name = Arg.Name;
    Sw->SwitchChar = Arg.SwitchChar;
    Sw->SwitchText = Arg.SwitchText;
    Sw->Description = Arg.Description;
    Sw->Required = Arg.Required;

    return *Sw;
}

Book::Enums::Code CArgs::Input(const std::vector<std::string_view>& StrArray)
{
    A = Args.end();
    for(auto Sv : StrArray)
    {
        auto Next = Query(Sv);

        AppBook::Debug() << Color::Cyan << "CArgs::Input" << Color::White << '(' << Color::Yellow << Sv << Color::White << ") :";

        if(Next == Args.end())
        {
            // It is not a switch - so must be an argument data for the CurArg/NextArg...
            if((A != Args.end()) && 
                ((*A)->Required > (*A)->Count) && ((*A)->Required > 0))
            {
                (*A)->Arguments.emplace_back(Sv);
                ++(*A)->Count;
                (*A)->Enabled = true;
                AppBook::Debug() << Color::Yellow << (*A)->Name << Color::Reset << " Arg:" << (*A)->Count << " '" << Sv << '\'';
            }
            else
            {
                // ... or non-switch arg
                Defaults.Arguments.emplace_back(Sv);
                ++Defaults.Count;
                Defaults.Enabled = true;
                AppBook::Debug() << Color::Yellow << Defaults.Name << Color::Reset << " Arg:" << Defaults.Count << " '" << Sv << '\'';
            }
        }
        else
        {
            if ((A != Args.end()) && ((*A)->Count < (*A)->Required))
            {
                AppBook::Error() << " Argument " << Color::Yellow << (*A)->Name << Color::Reset << " is missing " << Color::Red4 << (*A)->Required - (*A)->Count << Color::Reset << " Arguments / " << Color::Lime << (*A)->Required;
                return Book::Result::Failed;
            }
            A = Next;
            (*A)->Enabled = true;    
        }
    }
    return Book::Result::Ok;
}


Book::Action  CArgs::Process()
{
    auto R = Book::Action::End;
    if (!Args.empty())
    {
        for (auto* Arg : Args)
        {
            if (Arg->Enabled)
            {
                if (Arg->Required > Arg->Count)    
                {
                    Arg->Enabled = false;
                    throw AppBook::Exception()[AppBook::Except() << " Switch '" << Color::Yellow << Arg->Name
                        << Color::Reset << " is missing "
                        << Color::Red4 << Arg->Required - Arg->Count
                        << Color::Reset << " Arguments: Requires "
                        << Color::Lime << Arg->Required];
                    
                    // throw did not work? :
                    AppBook::Except() << " If this is shown, it means exceptions are deactivated. I suggest to enable it... ";
                    AppBook::Out() << Usage();
                    return Book::Action::Leave;

                }
                if (Arg->DelegateCB(*Arg) != Book::Action::Continue) 
                    return Book::Action::Leave;
            }
        }
    }
    if(!Defaults.Arguments.empty())
    {
        if(Defaults.Enabled)
        {
             R = Defaults.DelegateCB(Defaults);
             if (R != Book::Action::Continue)
                 return Book::Action::Leave;
        }
    }
    return Book::Action::Continue;
}

Switch &CArgs::operator[](const std::string &ArgName)
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



Switch::~Switch()
{
    Arguments.clear();
    DelegateCB.DisconnectAll();
}
}



































