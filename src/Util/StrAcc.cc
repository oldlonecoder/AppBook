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




#include "AppBook/Util/StrAcc.h"
#include <chrono>





StrAcc& StrAcc::operator += (const StrAcc& acc)
{
    _d += acc();
    return *this;
}


std::string::size_type StrAcc::ScanArg(std::string::size_type _pos)
{
    //std::cerr << "line:" << __LINE__<< ":ScanArg(" << Pos << ") : '" << _InputText << "'\n";
    _ArgPos = _d.find('%', _pos);
    if(_ArgPos != std::string::npos)
    {
        //std::cerr << "check _InputText[arg_pos+1]: _InputText[" << _ArgPos + 1 << "] : '" << _InputText[_ArgPos+1] << "'\n";

        if(_ArgPos < _d.length())
        {
            if(_d[_ArgPos + 1] == '%')
            {
                // forcément sur l'autre cas : '%' donc _InputText contient "%%" :
                _d.erase(_ArgPos, 1);
                std::cerr << "_InputText.erase(" << _ArgPos << ",1) : '" << _d << "'\n" <<
                          "=> _InputText[arg_pos+1] = '" << _d[_ArgPos + 1] << "'\n";
                std::cerr << "recurse at: " << _ArgPos + 1 << ":\n";
                return ScanArg(_ArgPos + 1);
            }
            //std::cerr << " return valid argument Position: " << _ArgPos << " > '" << _InputText << "'\n";
            return _ArgPos;
        }
        if(_d[_ArgPos + 1] == ' ')
            return ScanArg(_ArgPos + 1);

    }
    return std::string::npos;
}



StrAcc& StrAcc::operator , (const StrAcc& acc)
{
    _d += acc();
    return *this;
}

StrAcc& StrAcc::operator , (Utf::Glyph::Type Ic)
{
    _d += Utf::Glyph::Data[Ic];
    return *this;
}
StrAcc& StrAcc::operator , (Utf::AccentFR::Type Ac)
{
    _d += Utf::AccentFR::Data[Ac];
    return *this;
}

StrAcc& StrAcc::operator , (Color::Pair arg_)
{
    _d += arg_();
    return *this;
}

StrAcc::StrAcc()
{
}


StrAcc::~StrAcc()
{
    _d.clear();
}

StrAcc &StrAcc::fill(char c, int count)
{
    _d += std::string(count, c);
    return *this;
}

StrAcc& StrAcc::operator << (Color::Pair arg_)
{
    std::string dd = Color::AnsiBg(arg_.Bg);
    dd += Color::Ansi(arg_.Fg);
    return Format(dd);
}


StrAcc& StrAcc::operator<<(Color::Code arg_)
{
    return Format(Color::Ansi(arg_));
}

StrAcc &StrAcc::operator<<(const StrAcc &acc)
{
    return Format(acc());
}

StrAcc &StrAcc::operator<<(Utf::Glyph::Type Ic)
{
    return Format(Utf::Glyph::Data[Ic]);
}

StrAcc &StrAcc::operator<<(Utf::AccentFR::Type Ac)
{
    return Format(Utf::AccentFR::Data[Ac]);
}

void StrAcc::Clear()
{
    _d.clear();
}

std::string StrAcc::MakeStr(const char* B, const char* E)
{
    std::string Str;
    const char* C = B;
    if ((!B) || (!E) || (!*B) || (!*E))
        return Str;
    while (C <= E)
        Str += *C++;

    return Str;
}

std::string StrAcc::Now(const std::string& str_fmt)
{
    // Old archaic Code...
    // time_t rawtime;
    // struct tm* timeinfo;
    // char tmb[180];
    // std::time(&rawtime);
    // timeinfo = std::localtime(&rawtime);
    // std::strftime(tmb, 60, str_fmt.c_str(), timeinfo);
    // std::string _s = tmb;
    // //return tmb;


    auto current = std::chrono::system_clock::now();
    auto now = std::chrono::system_clock::to_time_t(current);
    auto local = localtime(&now);
    std::ostringstream ostr;
    ostr << std::put_time(local, str_fmt.c_str());
    return ostr.str();
}

StrAcc &StrAcc::operator+=(Utf::Glyph::Type Ic)
{
    _d += Utf::Glyph::Data[Ic];
    return *this;
}

StrAcc &StrAcc::operator+=(Utf::AccentFR::Type Ac)
{
    _d += Utf::AccentFR::Data[Ac];
    return *this;
}

StrAcc StrAcc::Make(const char *Str)
{
    return Str != nullptr ? Str : "";
}

std::vector<std::string_view> StrAcc::SVArray(int Count, char **s)
{
    std::vector<std::string_view> temp;
    for(int c = 1; c<Count; c++) temp.emplace_back(s[c]);

    return temp;
}

