//
// Created by oldlonecoder on 24-01-28.
//

/******************************************************************************************
 *   Copyright (C) /.../2024 by Serge Lussier                                             *
 *   lussier.serge@gmail.com                                                              *
 *                                                                                        *
 *   Unless otherwise specified, all code in this project is written                      *
 *   by the author (Serge Lussier)                                                        *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply to the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 *****************************************************************************************/

#pragma once
#include "AppBook/Book/AppBook.h"

//#ifndef APPBOOK_TEXTCURSOR_H
//#define APPBOOK_TEXTCURSOR_H


using namespace std::literals;
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

/*
class __attribute__ ((visibility ("hidden"))) MarkupProc
{

    struct Element
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

        enum class Mnemonic : uint16_t
        {
            Fg, Bg, Box, Pressed, Single, Double, Table, Row,Column,
            Color, Ic, ArgSeq, ArgSep,Underline,
            Identifier, OpenTag, Reset, ClosingTag, Br,
            Eos, AccentSeq, JustifySeq, Left, Top,
            Right, Bottom, Center, HVCenter, Null

        };


        // non-case-sensitive keywords/lexems:
        using Lexem                       = const char*;
        static constexpr Lexem Fg         = "FG";
        static constexpr Lexem Bg         = "BG";
        static constexpr Lexem ColorPair  = "COLOR";
        static constexpr Lexem Icon       = "ICON";
        static constexpr Lexem OpenSeq    = "<";
        static constexpr Lexem Reset      = "/";
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
        static constexpr Lexem Pushed     = "PUSHED";
        static constexpr Lexem Normal     = "NORMAL";
        static constexpr Lexem Single     = "SINGLE";
        static constexpr Lexem Double     = "DOUBLE";
        static constexpr Lexem Table      = "TABLE";
        static constexpr Lexem Row        = "ROW";
        static constexpr Lexem Coloumn    = "COLUMN";
        static constexpr Lexem Underline  = "U";
        static constexpr Lexem Null       = "Unset";

        Element::Type T = Element::Type::Unset;
        Element::Mnemonic M = Element::Mnemonic::Null;
        Element::Lexem L = nullptr;

        std::string operator()();

        static Element ScanToElement(const char* Start);

        using Array = std::vector <Element>;
        static Array Referential;

        Element() = default;

        Element(MarkupProc::Element::Type aType, MarkupProc::Element::Mnemonic aMenomic, MarkupProc::Element::Lexem aLexem, int nParams);
        Element(Element&&)noexcept;
        Element(const Element&) = default;

        Element& operator=(MarkupProc::Element&& r) noexcept
        {
            T = r.T;
            M = r.M;
            L = r.L;
            _Location = r._Location;
            return *this;
        }
        Element& operator=(const Element& r)
        {
            T = r.T;
            M = r.M;
            L = r.L;
            _Location = r._Location;
            return *this;
        }

        operator bool() { return (L != nullptr) || (T != Element::Type::Unset); }

        std::string Mark(const char*);
        int NParam = 0;

    };

};

*/



/*!
 * @brief Base Text processing... To be developped...
 *
 * @author  &copy; 2024, Serge Lussier (oldlonecoder, serge.lussier\@oldlonecoder.club)
 *
 * @todo <ul> <li> Embed Util::StrBreak ...</li>
 * <li>Remove STMLText from this library to create its own.</li>
 * <li>Remove the starting Test subdir to create its own library.</li>
 * </ul>
 */
class APPBOOK_EXPORTS SVScanner
{
    std::string_view Text{};

    std::string_view::iterator mEnd{};
    std::string_view::iterator mPos{};
    std::string_view::iterator mBegin{};


public:

    using Iterator = std::string_view::iterator;

    struct APPBOOK_EXPORTS  [[maybe_unused]] Context
    {
        std::string_view::iterator Pos{};
        std::string_view::iterator Begin{};
        Book::Result               Return{Book::Result::Notimplemented};
        //...
    };

    struct APPBOOK_EXPORTS Numeric
    {
    private:
        static SVScanner Empty;
    public:

        struct APPBOOK_EXPORTS Details
        {
            std::string_view Seq;
            enum class BaseSize : char
            {
                Binary,Octal,Decimal, Hexadecimal

            }Base{SVScanner::Numeric::Details::BaseSize::Decimal };
            enum class SizeType: char
            {
                I8,U8,I16,U16,I32,U32,I64,U64,F32,F64,F128
            }Size{SizeType::F64};
            bool Real{false};
            double Value;

            [[nodiscard]] bool I8()   const {return Size == SizeType::I8; }
            [[nodiscard]] bool U8()   const {return Size == SizeType::U8; }
            [[nodiscard]] bool I16()  const {return Size == SizeType::I16; }
            [[nodiscard]] bool U16()  const {return Size == SizeType::U16; }
            [[nodiscard]] bool I32()  const {return Size == SizeType::I32; }
            [[nodiscard]] bool U32()  const {return Size == SizeType::U32; }
            [[nodiscard]] bool I64()  const {return Size == SizeType::I64; }
            [[nodiscard]] bool U64()  const {return Size == SizeType::U64; }
            [[nodiscard]] bool F32()  const {return Size == SizeType::F32; }
            [[nodiscard]] bool F64()  const {return Size == SizeType::F64; }
            [[nodiscard]] bool F128() const {return Size == SizeType::F128; }

            [[nodiscard]] bool Decimal() const { return Base == BaseSize::Decimal; }
            [[nodiscard]] bool Binary() const { return Base == BaseSize::Binary; }
            [[nodiscard]] bool Octal() const { return Base == BaseSize::Octal; }
            [[nodiscard]] bool Hexadecimal() const { return Base == BaseSize::Hexadecimal; }
            [[nodiscard]] bool Float() const { return Real; }

            void ScaleValue();
        };

        using Result = std::pair<Book::Result, SVScanner::Numeric::Details>;

        SVScanner& Text{SVScanner::Numeric::Empty};

        std::string_view Seq{};

        std::string_view::iterator End{};
        std::string_view::iterator Pos{};
        std::string_view::iterator Begin{};

        bool Real{false};
        Details NumDetails{};
        Numeric() = default;
        explicit Numeric(SVScanner& Tx);


        ~Numeric() = default;
        Book::Result operator()();


        Book::Result Base2();
        Book::Result Base8();
        Book::Result Base10();
        Book::Result Base16();

        void Sign();
    };

public:

    struct APPBOOK_EXPORTS LocationData
    {
        // { \n; \n\r; \r\n; \r }
        std::size_t Line{0};
        std::size_t Col{0};
        std::size_t Offset{0};
        std::size_t PrevLine{0};
        std::size_t PrevCol{0};
        LocationData const & operator >> (std::string& Out) const;
    };

    SVScanner() = default;
    explicit SVScanner(std::string_view Txt);

    SVScanner(SVScanner&& ) noexcept = default;
    SVScanner(SVScanner&  ) = default;
    ~SVScanner() = default;

    SVScanner& operator = (SVScanner&&) noexcept = default;
    SVScanner& operator = (SVScanner const &) = default;
    SVScanner& operator = (std::string_view view);


    LocationData& Location() { return mLocation; }

    bool SkipWS();

    SVScanner::LocationData& Sync();

    bool Eof();
    bool Eof(std::string_view::iterator cc);

    Book::Result Seek(int32_t Idx);
    // void Seek(std::string_view::iterator IPos); ///< Disabled

    explicit operator bool() const;
    [[nodiscard]] inline bool Empty() const { return Text.empty() ;}

    auto operator*() const  { return *mPos; }
    auto operator()() const { return mPos; }


    //  -   Need more check and use cases...:
    //    auto  operator +( size_t sz );
    //    SVScanner&  operator += ( size_t sz );
    //------------------------------------------------
    bool         operator ++();
    bool         operator ++(int);

    SVScanner::Numeric::Result ScanNumber();
    std::pair<Book::Result, std::string_view> ScanLiteralString();

    void PushLocation();
    bool PopLocation();

    void Push();
    bool Pop();

    
    std::string Mark();

    // - Can only advance. For backward, juste save the current position.
    //    bool         operator --();
    //    bool         operator --(int);
    // -----------------------------------------------------------------------
    SVScanner::Iterator Begin();
    SVScanner::Iterator End();
<<<<<<< HEAD

    Book::Result Reposition(int Offset);
=======
>>>>>>> origin/main

private:

    LocationData mLocation{};
    std::vector<std::pair<std::size_t,std::size_t>> mPoints;
    std::stack<SVScanner::Iterator> PStack{};
};

}

//#endif //APPBOOK_TEXTCURSOR_H
