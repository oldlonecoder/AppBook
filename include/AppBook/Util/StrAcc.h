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

#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <cmath>
#include <functional>
#include <string>
#include <string_view>
#include <chrono>
#include <stack>
#include <AppBook/Util/Colors.h>
#include <AppBook/Utf/Glyphes.h>


class APPBOOK_API StrAcc final
{

    std::string _d; ///< private std::string instance;
    std::string::size_type _ArgPos = std::string::npos;
public:
    using Array = std::vector<std::string>;
    using CArray = std::vector<std::string_view>;
    using Stack = std::stack<std::string>;
    using Iterator = StrAcc::Array::iterator;
    using CIterator = StrAcc::Array::const_iterator;
private:
    /*!

        @ref https://en.cppreference.com/W/cpp/utility/format/format
    */
    // template <typename... Args> std::string _Format(std::string_view rt_fmt_str, Args&&... args)
    // {
    //     return std::vformat(rt_fmt_str, std::make_format_args(args...));
    // }

    // %[flags][Width][.precision][Length]specifier
    struct FomatData
    {
        uint8_t     F = 0; // Flag ( - + . # 0 ) => if Code='Code' then '-' => justify right; '+' => justify left; '^' => justify center.
        uint8_t     W = 0; // Width ( Length ) Value
        uint8_t     R = 0; // Custom flag set if this format requires floating Point spec.
        uint8_t     P = 6; // Precision (Same As  default).
        uint8_t     L = 0; // Length modifier ( linenum,ll,H,hh )
        std::size_t Len = 0; // Format Length.
        std::size_t Position = 0; // Arg Offset Position within _InputText.
        char        Code = 0; // Effective characeter Code specifier such As 'd'; 'Code'; 'f'; 'l'; 'p'...
        const char* StrPtr = nullptr;

        FomatData(std::string& str_) : StrPtr(str_.c_str())
        {}

        std::string operator()();
        operator std::string(){ return (*this)(); }

    };

    //std::string::size_type ScanArg(std::string::size_type _pos = 0);
    operator std::string(){ return (*this)(); }
    template<typename T> StrAcc& Format(const T& _argv);


public:

    StrAcc();
    StrAcc(const std::string& instr) { _d = instr;}
    StrAcc(const char* instr) { _d = instr;}
    StrAcc(std::string_view instr) { _d = instr.data();}


    ~StrAcc();
    std::string::size_type ScanArg(std::string::size_type _pos = 0);
    //...
    StrAcc& operator << (Color::Code arg_);
    StrAcc& operator << (Color::Pair arg_);

    template<typename T> StrAcc& operator >> (T& out)
    {
        std::istringstream in(_d); //  When String was derived from std::string ... Oops!  std::istringstream IsIn(*this);

        if constexpr ( std::is_same<float, T>::value || std::is_same<double, T>::value || std::is_same<long double, T>::value || std::is_same<int, T>::value || std::is_same<unsigned int, T>::value || std::is_same<int8_t, T>::value || std::is_same<uint8_t, T>::value || std::is_same<int16_t, T>::value || std::is_same<uint16_t, T>::value || std::is_same<int32_t, T>::value || std::is_same<uint32_t, T>::value || std::is_same<int64_t, T>::value || std::is_same<uint64_t, T>::value )
        {
            //IsIn.precision(_mPrecision);
            out = 0;
            //IsIn.precision(_mPrecision);
            in >> out;
            return *this;
        }
        T R;
        in >> R;
        out = R;
        return *this;
    }

    template<typename T> StrAcc& operator << (T arg_)
    {
        //if(ScanArg() != std::string::npos)
        return Format(arg_);

        //return this->operator+=(arg_);
    }

    StrAcc& operator = (const std::string& str) { _d = str; return *this; }
    StrAcc& operator = (const char* str) { _d = str; return *this; }
    StrAcc& operator = (char* str) { _d = str; return *this; }
    StrAcc& operator = (char cc) { _d = cc; return *this; }
    StrAcc& operator = (std::string_view str) { _d = str.data(); return *this; }
    StrAcc& operator = (std::string_view&& rstr) { _d = rstr.data(); return *this; }

    StrAcc& operator << (const StrAcc& acc);
    StrAcc& operator << (Utf::Glyph::Type Ic);
    StrAcc& operator << (Utf::AccentFR::Type Ac);
    StrAcc& operator << (std::string_view Ac)
    {
        return Format(std::string(Ac));
    }
    StrAcc& operator << (const std::string& Ac)
    {
        return Format(std::string(Ac));
    }

    StrAcc& fill(char c, int count);
    template<typename T> StrAcc& operator , (const T& arg_) { return (*this) += arg_; }
    template<typename T> StrAcc& operator + (const T& arg_)
    {
        return (*this) += arg_;
    }

    static StrAcc Make(const char* Str);
    bool   operator [] (const char* SubStr){ return _d.find(SubStr) != std::string::npos; }
    bool   operator !() { return _d.empty();}
    explicit operator bool() { return !_d.empty(); }

    //---------------------------------------------------------------------------------------------------------------
    // Explicit concatenations:
    StrAcc& operator += (std::string_view str) { _d += str.data(); return *this; }
    StrAcc& operator += (std::string_view&& rstr) { _d += rstr.data(); return *this; }
    StrAcc& operator += (const std::string& str) { _d += str; return *this; }
    StrAcc& operator += (const char* str) { _d += str; return *this; }
    StrAcc& operator += (char* str) { _d += str; return *this; }
    StrAcc& operator += (char cc) { _d += cc; return *this; }
    StrAcc& operator += (Color::Code c_) { _d += Color::Ansi(c_); return *this; }
    StrAcc& operator += (const StrAcc& acc);
    StrAcc& operator += (Utf::Glyph::Type Ic);
    StrAcc& operator += (Utf::AccentFR::Type Ac);

    StrAcc& operator , (std::string_view str) { _d += str.data(); return *this; }
    StrAcc& operator , (std::string_view&& rstr) { _d += rstr.data(); return *this; }
    StrAcc& operator , (const std::string& str) { _d += str; return *this; }
    StrAcc& operator , (const char* str) { _d += str; return *this; }
    StrAcc& operator , (char* str) { _d += str; return *this; }
    StrAcc& operator , (char cc) { _d += cc; return *this; }
    StrAcc& operator , (Color::Code c_) { _d += Color::Ansi(c_); return *this; }
    StrAcc& operator , (const StrAcc& acc);
    StrAcc& operator , (Utf::Glyph::Type Ic);
    StrAcc& operator , (Utf::AccentFR::Type Ac);
    StrAcc& operator , (Color::Pair arg_);
    //-----------------------------------------------------------------------------------------------------------------
    template<typename T> StrAcc& Hex(T& v)
    {
        std::stringstream is(_d.c_str() + 2);
        //std::cerr << " this:'" << _D.c_str()+2 << "' -> ";
        is >> std::hex >> v;
        //std::cerr << v << '\n';
        return *this;
    }

    template<typename T> StrAcc& operator += (const T& arg_)
    {
        std::ostringstream out;
        out << arg_;
        _d += out.str();
        return *this;
    }
    std::size_t Len() { return _d.length(); }
    std::string& StdStr() { return _d; }

    static std::string MakeStr(const char* B, const char* E);
    static std::string Now(const std::string& str_fmt);
    bool Empty() { return _d.empty(); }
    template<typename T> static std::string ToBinary(T _arg, bool padd = false, int f = 128)
    {
        uint8_t seq;
        int     nbytes = sizeof(T);

        uint8_t tableau[sizeof(T)];
        memcpy(tableau, (uint8_t*)&_arg, nbytes);
        std::string stream;
        int         s = 0;
        //bool Discard = false;
        for (int x = 1; x <= nbytes; x++)
        {
            seq = tableau[nbytes - x];
            if ((x == 1 && !padd && !seq) || (stream.empty() && !padd && !seq))
                continue;
            for (int y = 7; y >= 0; y--)
            { // est-ce que le bit #Y est à 1 ?
                if (s >= f)
                {
                    stream += ' ';
                    s = 0;
                }
                ++s;
                uint8_t b = 1 << y;
                if (b & seq)
                    stream += '1';
                else
                    stream += '0';
            }
        }
        /*tableau.Clear();*/
        return stream;
    }

    static std::vector<std::string_view> SVArray(int Count, char** s);

    const std::string& operator()() const { return _d; }


    void Clear();
};

#define MAX_STREAM_SIZE 200

template<typename T> StrAcc& StrAcc::Format(const T& _argv)
{
    Cycle:
    if(ScanArg() == std::string::npos)
        return this->operator+=(_argv);

    if(!std::isalnum(_d[_ArgPos+1]))
        return this->operator+=(_argv);

    StrAcc::FomatData fmt = {_d };
    char     buf[MAX_STREAM_SIZE];
    std::memset(buf, 0, MAX_STREAM_SIZE);
    //LFnl << "\n";

    // Commentaires &eacute;crits en anglais seulement pour simplifier le texte.
    std::string::iterator c = _d.begin() + _ArgPos;
    std::string::iterator n, beg, l;
    beg = n = c;
    ++c;
    // %[flag] :

    switch (*c)
    {
        case '-':
        case '+':
        case '#':
        case '0':fmt.F = *c++; break;
        case 'l':fmt.L = *c++; break;
        default:
            //++m;
            break;
    }

    n = c;
    // %[Width]:
    while ((n != _d.end()) && isdigit(*n))
        ++n;
    l = n; // save  ahead 'cursor Position'
    --n;
    if (n >= c)
    {
        int t = 0;
        while (n >= c)
            // interpret format Width value:
            fmt.W = fmt.W + (*(n--) - '0') * static_cast<int>(pow(10, t++));
    }
    else
        fmt.W = 0; // no Width

    c = l; // update effective cursor Position

    // check floating Point format: m,n,l:=>  read positions
    if (*c == '.')
    {
        fmt.R = fmt.P;
        ++c;
        n = c;
        while ((n != _d.end()) && isdigit(*n))
            ++n;
        l = n;
        --n;
        int t = 0;
        fmt.R = 0;
        while (n >= c)
            fmt.R = fmt.R + (*(n--) - '0') * static_cast<int>(pow(10, t++));
        c = l;
    }
    else
        fmt.R = fmt.P; // no floating Point format

    //[.precision]
    n = c;
    //% ([Length]) [specifier]
    // format prefix ends;
    // Now the format Type STMLMnemonic:
    //  - remove the format string.
    //  - insert 'formatted/interpreted' value substring at the _arg_position:
    //  - Reset _arg_position
    switch (*c)
    {
        case 'b':
        {
            if constexpr (!std::is_same<T, const std::string&>::value)
            {
                std::string BinaryStr;
                bool        pad = fmt.F == '0';
                BinaryStr = StrAcc::ToBinary<T>(_argv, pad,
                                                fmt.W <= 128 ? fmt.W : 128); // Limit grouping digits To 128 ...

                fmt.Len = (c + 1) - beg;  //save format substring'Code Length
                _d.erase(_ArgPos, fmt.Len);
                _d.insert(_ArgPos, BinaryStr.c_str(), BinaryStr.length());
                _ArgPos = std::string::npos;
                return *this;
            }
        }
            break;
        case 'd': // Decimale ou entier
        case 'D':
        case 'i':
        case 'I':fmt.Code = *c++;
            break;
        case 'x':
        case 'X':fmt.Code = *c++;
            break;
        case 'f':
        case 'F':
        case 'g':
        case 'G':fmt.Code = *c++;
            break;
        case 's':
        case 'S':
        case 'c':fmt.Code = *c++;
            break;
        default:
            // Reject, pass, To next :
            ++_ArgPos;
            goto Cycle;
    }

    fmt.Len = c - beg;
    //std::cout << __PRETTY_FUNCTION__ << '\n' << __LINE__ << " _D:\n'" << _D << "':\n";
    std::string ff(_d, _ArgPos, fmt.Len);
    //std::cout << "ff(_InputText, _ArgPos, fmt.Len): '" << ff << "'\n";
    // -- Clang-tidy:error: cannot pass Object of non-trivial Type 'const std::__cxx11::basic_string<char>' through variadic function
    //
    if constexpr (std::is_same<T, const std::string&>::value)
    {
        std::snprintf(buf, MAX_STREAM_SIZE, ff.c_str(), _argv.c_str());
        //std::cout << "\nbuffer[argv=const std::string&]:'" << buf << "'\n";
    }
    else if constexpr (std::is_same<T, std::string>::value)
    {
        std::snprintf(buf, MAX_STREAM_SIZE, ff.c_str(), _argv.c_str());
        //std::cout << "\nbuffer[argv=std::string]:'" << buf << "'\n";
    }
    else
    {
        std::sprintf(buf, ff.c_str(), _argv);
        //std::cout << "\nbuffer[argv=T (const T&)]:'" << buf << "'\n";
    }
    _d.erase(_ArgPos, fmt.Len);
    _d.insert(_ArgPos, buf, std::strlen(buf));
    _ArgPos = std::string::npos;
    return *this;
}







