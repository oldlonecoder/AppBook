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



#include "AppBook/Utf/Glyphes.h"
#include "AppBook/Util/StrAcc.h"
#include "AppBook/Book/BookEnums.h"
#include "AppBook/Util/Delegate.h"


namespace Book
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
 * \brief The STML (Simple Text Markup Language) STMLText class
 *
    \code
        Book::Enums::Code Test::TestText()
        {
            STMLText Text;
            std::string Encoded_Text;
            try{
                SMTLInputText = "#{fb:Reset; fg:Yellow; Icon:Home;} Salut #{fb:Reset Fg:White;  fg:yellow;} &agrave; #{fb:reset;}Toi!";

                auto RetCode = Text << STMLInputText >> ExtendedTextReceiver;

                // --> note: AppBook::Exception() [ AppBook::Error() << " In case of error, STMLText::Compiler will report and 'mark' the line/column" at the error location..." ];
                return Book::Result::Ok;
            }
            catch(Book::Exception& E)
            {
                std::cerr << "Exception caught : " << E.what() << '\n';
            }
        }
    \endcode

    \todo: Start implementing some "box" drawing...
    \code
        "Hello, <fg:White; box:single|bold> All! </box> -> ..."
                ┍━━━━━━━┓
         "Hello,│ Allo! ┃ -> ..."
                └───────┚

        "Hello, <fg:White; box:bold|single|Blue4; }> All! </box> -> ..."
                ┎───────┐
         "Hello,┃ Allo! │ -> ..."
                ┗━━━━━━━┙

    \endcode

 */

class APPBOOK_EXPORTS STMLText
{
    std::string _InputText;
    Color::Format _Format = Color::Format::ansi256;
    enum StmlStatus : char
    {
        Ok,
        Err
        //...
    } Status{STMLText::Ok};

public:
    STMLText();

    STMLText(const STMLText &other);

    STMLText(const std::string &aStr);

    STMLText(std::string &&aStr) noexcept;

    STMLText(const char *aStr);

    /**
        * Destructor
        */
    ~STMLText();

    Book::Enums::Code compile();

    STMLText &operator<<(const std::string &input_str);

    Book::Enums::Code operator>>(std::string &);

    Book::Enums::Code operator()();

    std::string InputText()
    { return _InputText; }

    void Clear();

    struct APPBOOK_EXPORTS STMLToken
    {
        enum class Type : uint8_t
        {
            Accent,
            AttrCmd,
            Identifier,
            Punctuation,
            Justify,
            Unset,
            Eof
        };

        enum class STMLMnemonic : uint16_t
        {
            Fg, Bg, Color, Ic, ArgSeq, ArgSep,
            Identifier, OpenTag, Reset, ClosingTag, Br,
            Eos, AccentSeq, JustifySeq, Left, Top,
            Right, Bottom, Center, HVCenter, Null
            /* ... */
        };

        struct LocationData
        {
            const char* Begin = nullptr;
            const char* End = nullptr;
            const char* StreamBegin = nullptr;
            int Index = -1;
        } _Location;

        // non-case-sensitive keywords/lexemes:
        using STMLLexem                       = const char*;
        static constexpr STMLLexem Fg         = "FG";
        static constexpr STMLLexem Bg         = "BG";
        static constexpr STMLLexem ColorPair  = "COLOR";
        static constexpr STMLLexem Icon       = "ICON";
        // static constexpr STMLLexeme OpenSeq    = "#{";
        static constexpr STMLLexem OpenSeq    = "<";
        static constexpr STMLLexem Reset      = "/";
        static constexpr STMLLexem CloseSeq   = ">";
        // static constexpr STMLLexeme CloseSeq   = "}";
        static constexpr STMLLexem ArgSeq     = ":";
        static constexpr STMLLexem ArgSep     = ",";
        static constexpr STMLLexem EoSt       = ";";
        static constexpr STMLLexem LineBreak  = "BR";
        static constexpr STMLLexem Accent     = "&";
        static constexpr STMLLexem Left       = "[";
        static constexpr STMLLexem Right      = "]";
        static constexpr STMLLexem Top        = "^";
        static constexpr STMLLexem Bottom     = "_";
        static constexpr STMLLexem Center     = "-"; // ex.: "<^-..." Top | Center
        static constexpr STMLLexem HVCenter   = "+";
        static constexpr STMLLexem Box        = "BOX";
        static constexpr STMLLexem Null       = "Unset";

        STMLToken::Type T = STMLToken::Type::Unset;
        STMLToken::STMLMnemonic M = STMLToken::STMLMnemonic::Null;
        STMLToken::STMLLexem L = nullptr;

        std::string operator()();

        static STMLToken ScanToToken(const char* Start);

        using Array = std::vector <STMLToken>;
        static Array Referential;

        STMLToken() = default;

        STMLToken(STMLText::STMLToken::Type aType, STMLText::STMLToken::STMLMnemonic aMenomic, STMLText::STMLToken::STMLLexem aLexem, int nParams);
        STMLToken(STMLToken&&)noexcept;
        STMLToken(const STMLToken&) = default;

        STMLToken& operator=(STMLText::STMLToken&& r) noexcept
        {
            T = r.T;
            M = r.M;
            L = r.L;
            _Location = r._Location;
            return *this;
        }
        STMLToken& operator=(const STMLToken& r)
        {
            T = r.T;
            M = r.M;
            L = r.L;
            _Location = r._Location;
            return *this;
        }

        operator bool() { return (L != nullptr) || (T != STMLToken::Type::Unset); }

        std::string Mark(const char*);
        int NParam = 0;

    };

    struct APPBOOK_EXPORTS  STMLAttribute
    {
        // ============ Indiquer c'est quoi qu'il faut lire!!!!!! ======================
        Color::Code FG  = Color::Black;
        Color::Code BG  = Color::Black;
        Utf::Glyph::Type Glph   = 0;
        Utf::AccentFR::Type Accent = Utf::AccentFR::Err;
        // =============================================================================
        const char* Begin = nullptr;
        const char* End   = nullptr;
        const char* Curs   = nullptr;

        struct Assigned
        {
            uint8_t f  : 1;
            uint8_t b  : 1;
            uint8_t g        : 1;
            uint8_t a      : 1;
        } _Assigned = {0};


        STMLAttribute() = default;
        STMLAttribute(STMLToken& Info);
        STMLAttribute(STMLAttribute&&) noexcept = default;
        STMLAttribute(const STMLAttribute&) = default;
        ~STMLAttribute() = default;
        std::string Infos();
        std::string operator()() const;
        operator bool() const { return Begin != nullptr; }
        STMLAttribute& operator = (STMLAttribute&&) noexcept = default;
        STMLAttribute& operator = (const STMLAttribute&) = default;
        using Array = std::vector<STMLText::STMLAttribute>;
    };

    const STMLText::STMLAttribute::Array& Attributes() const { return _Attributes; }
    void PushAttribute(STMLText::STMLAttribute Attr);

    // ------------------------ Notify Hooks ------------------------------------------------------------------
    Delegate<STMLText::STMLAttribute> AttributeDelegate;
    Delegate<char> OutputDelegate;
    template<typename T> Delegate<STMLText::STMLAttribute>::Slot Connect(T* cls, Book::Enums::Code (T::*fn)(STMLText::STMLAttribute))
    {
        return AttributeDelegate.Connect(cls, fn);
    }
    template<typename T> Delegate<char>::Slot Connect(T* cls, Book::Enums::Code (T::*fn)(char))
    {
        return OutputDelegate.Connect(cls, fn);
    }
    // --------------------------------------------------------------------------------------------------------

private:
    friend class Compiler;
    friend struct STMLAttribute;
    STMLText::STMLAttribute::Array _Attributes;

    struct Compiler
    {
        // Not really/correctly implemented:
        enum class Context : uint8_t {
            Out,        ///< Outside context Status ( normal STMLText ) - Begin Seq is Expected
            StartAttr,  ///< Enter Attributes context '<' - Entering into Seq Status (inside) context - Requires Out current Status
            EndAttr,    ///< EndSeq '<' Requires IntAttr current Status; Switch to Out
            InAttr,     ///< Attr Seq, requires BeginAttr current Status.
            AttrSeq,    ///< Command Token requires InAttr current Status.
            AttrArg,    ///< Command
            ArgSeq,     ///< Argument Seq, requires CmdToken current Status.
            StartUTF,   ///< UTF-8 a '&' requires Out current Status.
            EndUTF,     ///< Set right after ParseAccentMnemonic.
            EndSeq
        };

        // Context St = Context::Out; unused
        const char* B = nullptr;
        const char* C = nullptr;
        const char* E = nullptr;

        STMLToken::Array Tokens; ///< Sous r&eacute;serve...Pas certain d'en avoir besoin.

        bool operator ++(); ///< Prefix
        bool operator ++(int); ///< Postfix
        STMLText& TextRef;

        //void SyntaxError(STMLToken& atoken);
        //void ValueError(STMLToken& atoken);
        [[maybe_unused]] void CloseToken(STMLToken& Info);
        Book::Enums::Code Execute();
        STMLText::STMLAttribute CompileAttribute(STMLText::STMLAttribute& Attr);
        STMLText::STMLAttribute CompileAccent(STMLText::STMLAttribute& Attr);

        [[maybe_unused]] STMLText::STMLToken Scan();
        STMLText::STMLToken SkipToAttribute();
        Color::Code ColorID(STMLToken& Token);
        Utf::Glyph::Type GlyphID(STMLToken& Token);
        STMLText::STMLToken ScanIdentifier();
        Book::Enums::Code EatToken(STMLText::STMLToken& Token);
        Compiler() = delete;
        explicit Compiler(STMLText& aTextInstance);
        ~Compiler() = default;
        bool Eof();

        using ParserFnPtr = Book::Enums::Code(STMLText::Compiler::*)(STMLText::STMLAttribute&);
        using ParsersTableDictionary = std::unordered_map<STMLToken::STMLMnemonic, STMLText::Compiler::ParserFnPtr>;

        Book::Enums::Code CompileGlyph(STMLText::STMLAttribute& A);       // Parser for/pour ':' + 'IconID' + ';'
        Book::Enums::Code CompileFG(STMLText::STMLAttribute& A);         // Parser for/pour ':' + 'ColorID' + ';'
        Book::Enums::Code CompileBG(STMLText::STMLAttribute& A);          // Parser for/pour ':' + 'ColorID' + ';'
        Book::Enums::Code CompileColor(STMLText::STMLAttribute& A);      // Parser for/pour ':' + 'ColorID' + ',' + 'ColorID' + ';'
        Book::Enums::Code CompileBRKLine(STMLText::STMLAttribute& A);         // 'BR'; ( <br; .. > or/ou <br>)
        Book::Enums::Code CloseAttribute(STMLText::STMLAttribute& A);  // 'BR'; ( <br; .. > or/ou <br>)
        Book::Enums::Code CheckEofStream(STMLText::STMLAttribute& A);
        std::string Mark();
    };

    static STMLText::STMLToken Scan(STMLText::STMLToken::STMLMnemonic M);
};
}
