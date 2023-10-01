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



#include "chrtools/Icons.h"
#include "chrtools/accents.fr.h"
//#include "logbook/book_data.h"
#include "logbook/notify.h"

namespace book
{
    /*
    //    "─", //    "━", //    "│", //    "┃", //    "┌", //    "┍",
    //    "┎", //    "┏", //    "┐", //    "┑", //    "┒", //    "┓",
    //    "└", //    "┕", //    "┖", //    "┗", //    "┘", //    "┙",
    //    "┚", //    "┛", //    "├", //    "┝", //    "┞", //    "┟",
    //    "┠", //    "┡", //    "┢", //    "┣", //    "┤", //    "┥",
    //    "┦", //    "┧", //    "┨", //    "┩", //    "┪", //    "┫",
    //    "┬", //    "┭", //    "┮", //    "┯", //    "┰", //    "┱",
    //    "┲", //    "┳", //    "┴", //    "┵", //    "┶", //    "┷",
    //    "┸", //    "┹", //    "┺", //    "┻", //    "┼", //    "┽",
    //    "┾", //    "┿", //    "╀", //    "╁", //    "╂", //    "╃",
    //    "╄", //    "╅", //    "╆", //    "╇", //    "╈", //    "╉",
    //    "╊", //    "╋", //    "═", //    "║", //    "╒", //    "╓",
    //    "╔", //    "╕", //    "╖", //    "╗", //    "╘", //    "╙",
    //    "╚", //    "╛", //    "╜", //    "╝", //    "╞", //    "╟",
    //    "╠", //    "╡", //    "╢", //    "╣", //    "╤", //    "╥",
    //    "╦", //    "╧", //    "╨", //    "╩", //    "╪", //    "╫",
    //    "╬", //    "╭", //    "╮", //    "╯", //    "╰", //    "╴",
    //    "╵", //    "╶", //    "╷", //    "╸", //    "╹", //    "╺",
    //    "╻", //    "╼", //    "╽", //    "╾", //    "╿",
    */

using std::string;
using std::string_view;

/*!
 * \brief The stml (Simple Text Markup Language) text class
 *
    \code book::code Test::TestText()
        {
            text txt;
            txt = "<fb:Reset; f:Yellow; Icon:Home;> Salut <b:Reset F:White;  f:yellow;> &agrave; <fb:reset;>Toi!";
            txt.compile();

            return rem::ok;
        }
    \endcode

    \note Be careful because as of now the tests are not done!;
    \todo: Start implementing some "box" drawing...
    \code
        "Hello, <fg:White; box:single|bold> You! </box> -> ..."
               ┍━━━━━━┓
         Hello,│ You! ┃ -> ... // Pressed button effect! :) ( But no mouse !! hahaha )
               └──────┚

        "Hello, <fg:White; box:single|bold> You! </box> -> ..."
               ┎──────┐
         Hello,┃ You! │ -> ... // Pressed button effect! :) ( But no mouse !! hahaha )
               ┗━━━━━━┙

    \endcode

 */
class  BOOK_PUBLIC text
{
    std::string _d;
    chattr::format _f = chattr::format::ansi256;

public:
    text();
    text(const text& other);
    text(const std::string& aStr);
    text(std::string&& aStr) noexcept;
    text(const char* aStr);
    /**
        * Destructor
        */
    ~text();
    book::code compile();

    book::code operator >> (std::string&);
    book::code operator()();

    std::string text_str() { return _d; }
    std::string operator << (const std::string& input_str);
    void clear();

    struct  BOOK_PUBLIC token_data
    {
        enum class type : uint8_t
        {
            Accent,
            AttrCmd,
            Identifier,
            Punctuation,
            Justify,
            Unset,
            Eof
        };

        enum class mnemonic : uint16_t
        {
            Fg, Bg, Color, Ic, ArgSeq, ArgSep,
            Identifier, OpenTag, ClosingTag, Br,
            Eos, AccentSeq, JustifySeq, Left, Top,
            Right, Bottom, Center, HVCenter, Null
            /* ... */
        };

        struct location_data
        {
            const char* begin = nullptr;
            const char* end = nullptr;
            const char* s_begin = nullptr;
            int index = -1;
        } _location;

        // non-case-sensitive keywords/lexems:
        using Lexem                       = const char*;
        static constexpr Lexem Fg        = "FG";
        static constexpr Lexem Bg        = "BG";
        static constexpr Lexem ColorPair  = "COLOR";
        static constexpr Lexem Icon      = "ICON";
        static constexpr Lexem OpenSeq    = "<";
        static constexpr Lexem CloseSeq   = ">";
        static constexpr Lexem ArgSeq     = ":";
        static constexpr Lexem ArgSep     = ",";
        static constexpr Lexem EoSt       = ";";
        static constexpr Lexem LineBreak  = "BR";
        static constexpr Lexem Accent     = "&";
        static constexpr Lexem Left       = "[";
        static constexpr Lexem Right      = "]";
        static constexpr Lexem Top        = "^";
        static constexpr Lexem Bottom     = "_";
        static constexpr Lexem Center     = "-"; // ex.: "<^-..." Top | Center
        static constexpr Lexem HVCenter   = "+";
        static constexpr Lexem Box        = "BOX";
        static constexpr Lexem Null       = "Unset";

        token_data::type T = token_data::type::Unset;
        token_data::mnemonic M = token_data::mnemonic::Null;
        token_data::Lexem L = nullptr;

        std::string operator()();

        static token_data scan(const char* Start);

        using list = std::vector <token_data>;
        static list Referential;

        token_data() = default;

        token_data(text::token_data::type aType, text::token_data::mnemonic aMenomic, text::token_data::Lexem aLexem,int nParams);
        token_data(token_data&&)noexcept;
        token_data(const token_data&) = default;

        token_data& operator=(text::token_data&& r) noexcept
        {
            T = r.T;
            M = r.M;
            L = r.L;
            _location = std::move(r._location);
            return *this;
        }
        token_data& operator=(const token_data& r)
        {
            T = r.T;
            M = r.M;
            L = r.L;
            _location = r._location;
            return *this;
        }

        operator bool() { return (L != nullptr) || (T != token_data::type::Unset); }

        std::string mark(const char*);
        int NParam = 0;

    };

    struct BOOK_PUBLIC  attribute
    {
        // ============ Indiquer c'est quoi qu'il faut lire!!!!!! ======================
        color::code _fg  = color::Black;
        color::code _bg  = color::Black;
        Icon::Type _icn   = 0;
        Accent::Type _accnt = Accent::Err;
        // =============================================================================
        const char* _begin = nullptr;
        const char* _end   = nullptr;
        const char* _crs   = nullptr;

        struct assigned
        {
            uint8_t foreground  : 1;
            uint8_t background  : 1;
            uint8_t icon        : 1;
            uint8_t accent      : 1;
        } _assign = {0};


        attribute() = default;
        attribute(token_data& Info);
        attribute(attribute&&) noexcept = default;
        attribute(const attribute&) = default;
        ~attribute() = default;
        std::string informations();
        std::string operator()() const;
        operator bool() const { return _begin != nullptr; }
        attribute& operator = (attribute&&) noexcept = default;
        attribute& operator = (const attribute&) = default;
        using list = std::vector<text::attribute>;
    };

    const text::attribute::list& attributes() const { return _attributes; }
    void push_attribute(text::attribute Attr);

    // ------------------------ Notify Hooks ------------------------------------------------------------------
    book::notify<text::attribute> attr_hook;
    book::notify<char> output_hook;
    template<typename T> book::notify<text::attribute>::slot connect(T* cls, expect<> (T::*fn)(text::attribute))
    {
        return attr_hook.connect(cls, fn);
    }
    template<typename T> book::notify<char>::slot connect(T* cls, expect<> (T::*fn)(char))
    {
        return output_hook.connect(cls, fn);
    }
    // --------------------------------------------------------------------------------------------------------

private:
    friend class compiler;
    friend struct attribute;
    text::attribute::list _attributes;

    struct compiler
    {
        // Not really/correctly implemented:
        enum class State : uint8_t {
            Out,        ///< In outside context state ( normal text ) - Begin Seq is Expected
            StartAttr,  ///< Enter Attributes context '<' - Entering into Seq state (inside) context - Requires Out current state
            EndAttr,    ///< EndSeq '<' Requires IntAttr current state; Switch to Out
            InAttr,     ///< in Attr Seq, requires BeginAttr current state.
            AttrSeq,    ///< Command Token requires InAttr current state.
            AttrArg,    ///< Command
            ArgSeq,     ///< Argument Seq, requires CmdToken current state.
            StartUTF,   ///< UTF-8 accent '&' requires Out current state.
            EndUTF,     ///< Set right after ParseAccentMnemonic.
            EndSeq
        };

       // State St = State::Out; unused
        const char* B = nullptr;
        const char* C = nullptr;
        const char* E = nullptr;

        token_data::list Tokens; ///< Sous r&eacute;serve...Pas certain d'en avoir besoin.

        bool operator ++(); ///< Prefix
        bool operator ++(int); ///< Postfix
        text& TextRef;

        void syntax_error(token_data& atoken);
        void value_error(token_data& atoken);
        void close_token(token_data& Info);
        book::code execute();
        text::attribute compile_attribute(text::attribute& Attr);
        text::attribute compile_accent(text::attribute& Attr);
        text::token_data scan();
        text::token_data skip_to_attribute();
        color::code color_id(token_data& Token);
        Icon::Type icon_id(token_data& Token);
        text::token_data scan_identifier();
        book::code eat_token(text::token_data& Token);
        compiler() = delete;
        explicit compiler(text& aTextInstance);
        ~compiler() = default;
        bool eof();

        using parser_fnptr_t = book::code(text::compiler::*)(text::attribute&);
        using parsers_table_t = std::unordered_map<token_data::mnemonic, text::compiler::parser_fnptr_t>;

        book::code parse_icon(text::attribute& A);       // Parser for/pour ':' + 'IconID' + ';'
        book::code parse_fg(text::attribute& A);         // Parser for/pour ':' + 'ColorID' + ';'
        book::code ParseBg(text::attribute& A);          // Parser for/pour ':' + 'ColorID' + ';'
        book::code parse_color(text::attribute& A);      // Parser for/pour ':' + 'ColorID' + ',' + 'ColorID' + ';'
        book::code parse_br(text::attribute& A);         // 'BR'; ( <br; .. > or/ou <br>)
        book::code close_attribute(text::attribute& A);  // 'BR'; ( <br; .. > or/ou <br>)
        book::code check_eos(text::attribute& A);
        std::string mark();
    };

    static text::token_data scan(text::token_data::mnemonic M);
};
}
