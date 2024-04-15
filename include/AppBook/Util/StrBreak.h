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
#include "AppBook/Util/StrAcc.h"



//using std::string;
using std::string;

class APPBOOK_API StrBreak
{
    static constexpr std::string_view mDefSeparators { "\\%(){}[]`$#@!;,~?^&<>=+-*/:." };
    using Iterator = std::string_view::iterator;
    using Array = std::vector<std::string>;

public:

    enum Opt
    {
        Keep,
        Discard
    };

    StrBreak() = default;

    ~StrBreak() = default;

    struct APPBOOK_API Word
    {
        StrBreak::Iterator Begin{};
        StrBreak::Iterator End{};
        StrBreak::Iterator Eos{};

        std::string operator()() const;
        std::string operator*() const;

        using Array = std::vector<StrBreak::Word>;
        using Iterator [[maybe_unused]] = StrBreak::Word::Array::iterator;
        [[maybe_unused]][[nodiscard]] std::string Mark() const;

        int Line{1};
        int Column{1};
        std::size_t Offset{0};
        std::string Location() const;
    };

    struct APPBOOK_API ConfigData
    {
        std::string_view View;
        std::string_view Delimiters;
        StrBreak::Opt o{ Opt::Keep };
        StrBreak::Word::Array Words;
    };

    //ConfigData& config() { return data; }
//    std::size_t operator()(StrBreak::Word::Array &Collection, std::string aDelimiters = "", StrBreak::Opt aopt = StrBreak::Keep) const;
    //std::size_t operator()();
    std::size_t operator()(StrBreak::ConfigData &dt);

    StrBreak::Iterator ScanTo(StrBreak::Iterator start_, char c_) const;

private:
    struct SBContext
    {
        std::string_view::iterator Begin{};
        std::string_view::iterator Pos{};
        std::string_view::iterator End{}; /// ...

        int _line{1};
        int _col{1};
        uint64_t Index{0};

        SBContext() = default;

        ~SBContext() = default;

        explicit SBContext(std::string_view str_);

        bool SkipWS();

        [[nodiscard]] bool AtEnd() const;

        bool operator++();

        bool operator++(int);

        void Reset(std::string_view str_)
        {
            Pos = Begin = str_.begin();
            _line = _col = 1;
            Index = 0;
            End = str_.end();
        }

        int ScanStr();

        SBContext &operator>>(StrBreak::Word &word_);
    } Cursor;

    static bool Append(SBContext &cursor, StrBreak::ConfigData &dat, const StrBreak::Word &w);

private:
    ConfigData Data;

};

