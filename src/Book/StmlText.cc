/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply       *
 ***************************************************************************/

#include "AppBook/Book/StmlText.h"

#include <map>
#include <AppBook/Book/AppBook.h>



namespace Book
{


//--------------------------------------------------------------------------------------------------------------------------------------------


// Referentiel des tokens du processeur :
STMLText::STMLToken::Array STMLText::STMLToken::Referential =
    {
        {STMLText::STMLToken::Type::Punctuation, STMLText::STMLToken::STMLMnemonic::OpenTag,    STMLText::STMLToken::OpenSeq,   0},
        {STMLText::STMLToken::Type::Punctuation, STMLText::STMLToken::STMLMnemonic::ClosingTag, STMLText::STMLToken::CloseSeq,  0},
        {STMLText::STMLToken::Type::Punctuation, STMLText::STMLToken::STMLMnemonic::ArgSeq,     STMLText::STMLToken::ArgSeq,    0},
        {STMLText::STMLToken::Type::Punctuation, STMLText::STMLToken::STMLMnemonic::ArgSep,     STMLText::STMLToken::ArgSep,    0},
        {STMLText::STMLToken::Type::Punctuation, STMLText::STMLToken::STMLMnemonic::Eos,        STMLText::STMLToken::EoSt,      0},
        {STMLText::STMLToken::Type::Punctuation, STMLText::STMLToken::STMLMnemonic::Reset,      STMLText::STMLToken::Reset,     0},
        {STMLText::STMLToken::Type::AttrCmd,     STMLText::STMLToken::STMLMnemonic::Color,      STMLText::STMLToken::ColorPair, 2},
        {STMLText::STMLToken::Type::AttrCmd,     STMLText::STMLToken::STMLMnemonic::Fg,         STMLText::STMLToken::Fg,        1},
        {STMLText::STMLToken::Type::AttrCmd,     STMLText::STMLToken::STMLMnemonic::Bg,         STMLText::STMLToken::Bg,        1},
        {STMLText::STMLToken::Type::AttrCmd,     STMLText::STMLToken::STMLMnemonic::Ic,         STMLText::STMLToken::Icon,      1},
        {STMLText::STMLToken::Type::AttrCmd,     STMLText::STMLToken::STMLMnemonic::Br,         STMLText::STMLToken::LineBreak, 0},
        {STMLText::STMLToken::Type::Accent,      STMLText::STMLToken::STMLMnemonic::AccentSeq,  STMLText::STMLToken::Accent,    1}
        //...
    };

// Parsers:




STMLText::STMLText()
= default;

STMLText::STMLText(const STMLText& other)
{
    _InputText = other._InputText;
}

STMLText::STMLText(const std::string& Str) : _InputText(Str) {}

STMLText::STMLText(std::string&& Str) noexcept : _InputText(std::move(Str)) {}

STMLText::STMLText(const char* aStr) : _InputText(aStr) {}

STMLText::~STMLText()
{
    _InputText.clear();
    _Attributes.clear();
}



STMLText::STMLToken STMLText::STMLToken::ScanToToken(const char* Start)
{
    ////Book::debug() << " Entering with Start :[" << *Start << "] -> '" << Start << "':";
    if (!*Start)
        return {};

    for (auto TokenRef : STMLText::STMLToken::Referential)
    {
        TokenRef._Location.StreamBegin = Start;
        while (isspace(*Start)) ++Start;

        auto crs = Start;
        const char* rtxt = TokenRef.L;
        //auto sz = std::strlen(rtxt);

        if (toupper(*crs) != toupper(*rtxt)) continue;

        while (*rtxt && *crs && (toupper(*crs) == toupper(*rtxt))) { ++crs; ++rtxt; }
        ////Book::debug() << "Token.L = [" << Color::Yellow << TokenRef.L << Color::Reset << "]:";
        if (!*rtxt)
        {   // fin de Token.L :
            if (TokenRef.T == STMLToken::Type::AttrCmd)
            {
                // Si le mot continue alors ce n'est pas un Token:
                if (*crs)
                {   // *crs n'est pas un Type d'espace:
                    if (isalnum(*crs))
                        continue;
                }
            } // Il reste les autres non-espace comme les ponctuations, symboles...
            --crs; // Replacer crs sur le dernier caractere du Token.
            //// Book::Enums::Code::push_debug() << " Scanned to :'" << Color::Yellow << *crs << Color::Reset << '\'';
            TokenRef._Location.Begin = Start;
            TokenRef._Location.End = crs; // Fin du Token

            return TokenRef; // On le retourne.... doh!
        }
    }
    // Book::Enums::Code::push_debug() << " '" << *Start << "' not a TokenRef";
    return {}; // crs ( ou Start )  n'est pas sur un token du referentiel.
}


STMLText::STMLToken::STMLToken(STMLText::STMLToken::Type aType, STMLText::STMLToken::STMLMnemonic aMnenomic, STMLText::STMLToken::STMLLexem aLexem, int nParams) :
    T(aType), M(aMnenomic), L(aLexem),NParam(nParams)
{

}

STMLText::STMLToken::STMLToken(STMLText::STMLToken&& r) noexcept :
    T(r.T), M(r.M), L(r.L)
{

    _Location = r._Location;
    //Book::Enums::Code::Debug(SourceLocation) << " ->[" << (*this)() << "]";
}

std::string STMLText::STMLToken::Mark(const char* Stream)
{
    if (!*this)
        return " - Can't Mark an invalid token...";
    const char* B = _Location.Begin;
    const char* E = _Location.End;
    while ((B >= Stream) && (*B != '\n')) --B;
    ++B;
    while (*E && (*E != '\n')) ++E;
    if(!E) --E;

    std::string MStr = std::string(_Location.Begin - Stream, ' ');
    MStr += Utf::Glyph::CArrowUp;
    StrAcc Str;
    Str << Color::Grey78 << std::string(B, E - B) << '\n' << Color::Yellow << MStr << Color::Reset;
    return Str();
}

Book::Enums::Code STMLText::compile()
{
    ////Book::debug() << " :";
    STMLText::Compiler Parse{*this };
    return Parse.Execute();
}

Book::Enums::Code STMLText::operator>>(std::string& Out)
{
    ////Book::debug() << " Building the STMLText using the processed data:";
    if(Status == STMLText::Err)
    {
        AppBook::Error() << " Bad Status error.";
        return Book::Enums::Code::Failed;
    }

    const char* r = _InputText.c_str();
    auto l = _InputText.length();
    const char* b = r;

    std::string AttrStr;

    for (auto const& A : _Attributes)
    {
        //Book::debug() << " Looping Attributes: '" << A() << "' length:" << A().length();
        while (r != A.Begin)
        {
            Out += *r;
            if(!OutputDelegate.Empty())
            {
                auto R = OutputDelegate(*r);
                if(R != Book::Action::Continue) return Book::Result::Rejected;
            }
            r++;
        }
        r += A().length();
        if(!AttributeDelegate.Empty())
        {
            auto R = AttributeDelegate(A);
            if(R != Book::Action::Continue) return Book::Result::Rejected;
        }

        // ----------- Foreground and b assign colors: -----------------------------
        if(A._Assigned.f && A._Assigned.b)
        {
            Color::Pair{A.FG, A.BG} >> AttrStr;
            Out += AttrStr;
        }
        else
        {

            if (A._Assigned.b)
            {
                AttrStr = _Format == Color::Format::ansi256 ? Color::AnsiBg(A.BG) : "Color::Format::html>::bg(A.BG)";
                Out += AttrStr;
            }
            else
            if (A._Assigned.f)
            {
                //Book::debug() << " f color #: " << static_cast<int>(A.FG);
                AttrStr = _Format == Color::Format::ansi256 ? Color::Ansi(A.FG) : "attr<Color::Format::html>::fg(A.FG)";
                Out += AttrStr;
            }
        }
        // ----------- Foreground and b assign colors End -----------------------------

        // ----------- Graphens and Accents: -----------------------------
        if (A._Assigned.a)
        {
            AttrStr = Utf::AccentFR::Data[A.Accent];
            Out += AttrStr;
        }
        else
        {
            if (A._Assigned.g)
            {
                AttrStr = Utf::Glyph::Data[A.Glph];
                Out += AttrStr;
            }
            //...
        }
        // ------------------------------------------------------------------

        //--------- Text commands : -------------------------------------------------
        //...To be continued...
    }
    while (static_cast<size_t>(r - b) < l)
    {
        OutputDelegate(*r);
        Out += *r++;
    }
    return Book::Enums::Code::Success;
}

Book::Enums::Code STMLText::operator()()
{
    if(OutputDelegate.Empty() || AttributeDelegate.Empty() ) return Book::Enums::Code::Rejected;
    if(compile() != Book::Enums::Code::Accepted) return Book::Enums::Code::Rejected;

    const char* r = _InputText.c_str();
    auto l = _InputText.length();
    const char* b = r;

    std::string AttrStr;

    for (auto const& A : _Attributes)
    {
        while (r != A.Begin)
        {
            auto R = OutputDelegate(*r);
            if(R != Book::Action::Continue)
                return Book::Result::Rejected;
            r++;
        }
        r += A().length();
        auto R = AttributeDelegate(A);
        if(R != Book::Action::Continue)
            return Book::Result::Rejected;
    }
    while (static_cast<size_t>(r - b) < l) OutputDelegate(*r++);

    return Book::Enums::Code::Accepted;
}



Book::STMLText& STMLText::operator<<(const std::string& input_str)
{
    Clear();
    _InputText = input_str;
    //Book::debug() << " calling STMLText::compile():";
    auto c = compile();
    if(c != Book::Enums::Code::Accepted)
    {
        AppBook::Error() << c;
        Status = STMLText::Err;
    }
    return *this;
}

void STMLText::Clear()
{
    _InputText.clear();
    _Attributes.clear();
}

void STMLText::PushAttribute(STMLText::STMLAttribute Attr)
{
    _Attributes.push_back(Attr);
}

STMLText::STMLToken STMLText::Scan(STMLText::STMLToken::STMLMnemonic M)
{
    for (auto iM : STMLText::STMLToken::Referential)
    {
        if (iM.M == M) return iM;
    }
    return {};
}


//     STMLText::Attribute::Attribute(const char* Start)
//     {
//         Begin = C = End = Start;
//     }

std::string STMLText::STMLToken::operator()()
{
    if (L == nullptr)
        return { "null" };

    return {_Location.Begin, (std::size_t)(_Location.End - _Location.Begin) + 1 };
}

STMLText::Compiler::Compiler(STMLText& aTextInstance) :
    B(aTextInstance._InputText.c_str()), C(aTextInstance._InputText.c_str()), E(aTextInstance._InputText.c_str()), TextRef(aTextInstance)
{
    E = B + aTextInstance._InputText.length() - 1;
}

bool STMLText::Compiler::operator ++()
{
    return false;
}

bool STMLText::Compiler::operator ++(int)
{
    return false;
}

//Book::Enums::Code STMLText::Compiler::Skip()
//{
//    //Book::Enums::Code::Debug() << " STMLText::TextParser::Skip(" << Color::Yellow << *C << Color::Reset << "):";
//    while (C <= E)
//    {
//        ++C;
//        if (!isspace(*C))
//            return Book::Enums::Code::Book::Enums::Code::Accepted;
//    }
//    return Book::Enums::Code::Book::Enums::Code::Eof;
//}



[[maybe_unused]] void STMLText::Compiler::CloseToken(STMLToken& Info)
{
    Info._Location.Index = static_cast<int>(Info._Location.End - B);
    auto sz = Info._Location.End - Info._Location.Begin;
    C += sz + 1;

    // Book::Enums::Code::push_debug({}) << " Token :" << Book::Enums::Code::endl << Info.Mark(B);

}

[[maybe_unused]] STMLText::STMLToken STMLText::Compiler::Scan()
{
    auto Token = STMLText::STMLToken::ScanToToken(C);
    if (!Token)
    {
        auto R = ScanIdentifier();
        if (!R)
        {
            // Book::Enums::Code::push_syntax({}) << " In STMLText::Compiler::ScanToToken :" << Book::Enums::Code::endl << Token.Mark(TextRef._InputText.c_str());
            return {};
        }
    }
    return Token;
}


/*!
    @brief Compile le texte pour creer les attributs :

    @code
        Compile :
        loop:
            SkipToAttr
            CompileAttribute
        loop
    @endcode
*/
Book::Enums::Code STMLText::Compiler::Execute()
{
    //Book::debug() << ":";
    while (!Eof())
    {

        auto Token = SkipToAttribute();
        STMLText::STMLAttribute Attr;
        STMLText::STMLAttribute A;
        if (!Token) return Book::Enums::Code::Accepted;
        if (Token.M == STMLText::STMLToken::STMLMnemonic::AccentSeq)
        {
            Attr.Begin = Token._Location.Begin;
            A = CompileAccent(Attr);
        }
        else if (Token.M == STMLText::STMLToken::STMLMnemonic::OpenTag)
        {
            Attr.Begin = Token._Location.Begin;
            A = CompileAttribute(Attr);
        }

        if (!A)
        {
            // Book::Enums::Code::push_debug() << " No STMLAttribute. Skipping to next \"OpenTag | AccentSeq\"...";
            //Skip();
            continue;
        }
        TextRef.PushAttribute(Attr);

    }
    return Book::Enums::Code::Accepted;
}

/*!
    @brief Compiler une sequence d'attribut(s).

    Ici c'est moins &eacute;vident... On d&eacute;coupe ce qui est entre '<' et '>' en commandes d'attribut.
    //...
    // '<' 'Fg' ':' 'ColorID' ';'...'Color' : 'ColorID(Fg)' ',' 'ColorID(Bg)' ';' 'Reset' ';' '>'

    @note  "Lexing And Parsing... tout &ccedile;a en m&ecirc;me temps!".
*/
STMLText::STMLAttribute STMLText::Compiler::CompileAttribute(STMLText::STMLAttribute& Attr)
{

    STMLText::STMLToken Token;
    // On tokenize les elements d'attribut: ( Stop: ClosingTag('>') Token )
    std::map<STMLText::STMLToken::STMLMnemonic, STMLText::Compiler::ParserFnPtr> Parsers =
        {
            {STMLText::STMLToken::STMLMnemonic::Fg, &STMLText::Compiler::CompileFG},
            {STMLText::STMLToken::STMLMnemonic::Bg, &STMLText::Compiler::CompileBG},
            {STMLText::STMLToken::STMLMnemonic::Color, &STMLText::Compiler::CompileColor},
            {STMLText::STMLToken::STMLMnemonic::Ic, &STMLText::Compiler::CompileGlyph},
            //... Plus tard, on aura les Grilles et Fenetres
            {STMLText::STMLToken::STMLMnemonic::ClosingTag, &STMLText::Compiler::CloseAttribute},
            {STMLText::STMLToken::STMLMnemonic::Br, &STMLText::Compiler::CompileBRKLine},
        };
    while (!Eof())
    {
        // on passe '<'
        //Skip();
        Book::Enums::Code ER;
        // Expecting STMLText::STMLToken::STMLMnemonic:
        Token = STMLText::STMLToken::ScanToToken(C);
        if (!Token)
        {
            AppBook::Syntax() << " Expected ACM (Attribute Command STMLMnemonic) Token - Returning Empty Attribute.";
            return {};
        }
        // Ici c'est obligatoire de faire une boucle qui teste explicitement les mnemonics sp&eacute;cifiques
        // parce qu'on ne prend en charge tous les mnemonics.
        for (auto [M, Fn] : Parsers)
        {
            if (M != Token.M) continue;
            EatToken(Token);
            ER = (this->*Fn)(Attr);
            if (ER != Book::Enums::Code::Accepted) return {};
            ////Book::debug() << " Token : '" << Token.L  << "' - STMLAttribute-contruct[" << Attr() << Color::White << "]";
            break;
        }

        if (Token.M == STMLText::STMLToken::STMLMnemonic::ClosingTag)
        {
            //EatToken(Token);
            Attr.End = Token._Location.End;
            return Attr; // Peut &ecirc;tre vide si on es sur "<>"
        }
    }
    AppBook::Syntax() << " Unexpected End of stream in Attribute parsing - Returning Empty Attribute";
    return {};
}



/*!
    @brief Cr&eacute;&eacute; un Attribute de texte Accent.

    @note Encoder un Accent est le plus facile, donc pas besoin d'accumuler les tokens ici: juste "parser" '&' + Id + ';' c'est tout!
*/
STMLText::STMLAttribute STMLText::Compiler::CompileAccent(STMLText::STMLAttribute& Attr)
{

    STMLText::STMLToken Token;
    //Skip();
    if(Eof())
    {
        // Book::Enums::Code::push_syntax() << "STMLText::Compiler::CompileAccent :  Expected identifier.";
        return {};
    }

    Token = ScanIdentifier();
    if (!Token)
    {
        // Book::Enums::Code::push_syntax() << " Expected identifier." << Book::Enums::Code::endl << Mark();
    }

    // Book::Enums::Code::push_debug() << " Identifier: " << Token();

    Utf::AccentFR::Type T = Utf::AccentFR::GetEnum(Token());
    if (T == Utf::AccentFR::Err)
    {
        // Book::Enums::Code::push_syntax() << " Unknown Code token(identifier) " << Book::Enums::Code::endl << Token.Mark(B);
        return {};
    }


    Attr = STMLText::STMLAttribute(Token);
    Attr.Accent = T;
    Attr._Assigned.a = 1;
    //Mandatory Expect ';'
    (void) EatToken(Token);
    if(CheckEofStream(Attr) != Book::Enums::Code::Accepted) return Attr;
    // Book::Enums::Code::push_syntax() << " Expected Eos ';' (End Of Statement token)." << Book::Enums::Code::endl << Mark();
    return {};
}

/*!
    @brief Tokenize jusqu'&agrave; '>' ou ';', puis Compiler l'attribut.
*/

STMLText::STMLToken STMLText::Compiler::SkipToAttribute()
{
    STMLText::STMLToken Token;

    while (!Eof())
    {
        auto c = toupper(*C);
        if (c != STMLToken::OpenSeq[0])
        {
            if (c != STMLToken::Accent[0])
            {
                ++C;
                continue;
            }
            Token.T = STMLText::STMLToken::Type::Accent;
            Token.L = STMLText::STMLToken::Accent;
            Token.M = STMLText::STMLToken::STMLMnemonic::AccentSeq;
        }
        else
        {
            Token.T = STMLText::STMLToken::Type::AttrCmd;
            Token.L = STMLText::STMLToken::OpenSeq;
            Token.M = STMLText::STMLToken::STMLMnemonic::OpenTag;
        }
        Token._Location.Begin = Token._Location.End = C;
        Token._Location.Index = static_cast<int>(C - TextRef._InputText.c_str());
        EatToken(Token);
        return Token;
    }
    return {};
}

bool STMLText::Compiler::Eof() { return C > E; }

Book::Enums::Code STMLText::Compiler::CompileGlyph(STMLText::STMLAttribute& A)
{
    auto Token = STMLText::STMLToken::ScanToToken(C);
    if (Token.T != STMLText::STMLToken::Type::Punctuation)
    {
        // Book::Enums::Code::push_syntax() << " Expected Punctuation token ':'" << Book::Enums::Code::endl << Mark();
        return Book::Enums::Code::Rejected;
    }
    EatToken(Token);
    Token = ScanIdentifier();
    auto R = GlyphID(Token);
    if (R == 0)
    {
        // Book::Enums::Code::push_syntax({}) << Book::Enums::Code::notexist << " Icon ID:\n" << Book::Enums::Code::endl << Token.Mark(B);
        return Book::Enums::Code::Rejected;
    }
    A.Glph = R;
    A._Assigned.g = 1;
    return CheckEofStream(A);
}



Book::Enums::Code STMLText::Compiler::CompileFG(STMLText::STMLAttribute& A)
{
    // C sur 'Fg'; ( Consomm&eacute; )
    // Attendus :  ':' , 'ColorID', '; | >';

    auto Token = STMLText::STMLToken::ScanToToken(C);
    //Book::debug() << Token();
    if(Token.L == STMLText::STMLToken::Reset)
    {
        //Book::debug() << " Explicit assign fg:Reset to STMLAttribute.";
        EatToken(Token);
        A._Assigned.f = 1;
        A.FG = Color::Reset; return Book::Enums::Code::Accepted;
    }

    if ((Token.T != STMLToken::Type::Punctuation) || (Token.L != STMLText::STMLToken::ArgSeq))
    {
        AppBook::Syntax() << " Expected token ':' " << Book::Enums::Fn::Endl << Mark();
        return Book::Enums::Code::Unexpected;
    }
    if(Token.L == STMLText::STMLToken::Reset)
    {
        //Book::debug() << "Reset: check...";
        EatToken(Token);
        return Book::Enums::Code::Accepted;
    }

    EatToken(Token);
    Token = ScanIdentifier();
    if (!Token)
    {
        AppBook::Syntax() << " Expected Identifier token." << Book::Enums::Fn::Endl << Mark();
        return Book::Enums::Code::Expected;
    }

    auto cid = ColorID(Token);
    if (cid == Color::Reset ) return Book::Enums::Code::Accepted;
    A.FG = cid;
    A._Assigned.f = 1;
    //Book::out() << " Compiler::ParseFg - Token:" << Book::Enums::Fn::Endl << Token.Mark(B);
    return CheckEofStream(A);
}

Book::Enums::Code STMLText::Compiler::CompileBG(STMLText::STMLAttribute& A)
{
    // C sur 'Fg'; ( Consomm&eacute; )
    // Attendus :  ':' , 'ColorID', '; | >';
    auto Token = STMLText::STMLToken::ScanToToken(C);
    if(Token.L == STMLText::STMLToken::Reset)
    {
        //Book::debug() << " Explicit assign fg:Reset to STMLAttribute.";
        EatToken(Token);
        A._Assigned.b = 1;
        A.BG = Color::Reset;
        return Book::Enums::Code::Accepted;
    }

    if ((Token.T != STMLToken::Type::Punctuation) || (Token.L != STMLText::STMLToken::ArgSeq))
    {
        // Book::Enums::Code::push_syntax({}) << Book::Enums::Code::Rejected << " Expected token ':'" << Book::Enums::Code::endl << Mark();
        return Book::Enums::Code::Rejected;
    }
    EatToken(Token);
    Token = ScanIdentifier();
    if (!Token)
    {
        // Book::Enums::Code::push_syntax({}) << Book::Enums::Code::Rejected << " Expected Identifier token." << Book::Enums::Code::endl << Mark();
        return Book::Enums::Code::Rejected;
    }

    auto cid = ColorID(Token);
    if (cid == Color::Reset) return Book::Enums::Code::Accepted;
    A.BG = cid;
    A._Assigned.b = 1;
    //Book::Enums::Code::out() << " Compiler::ParseFg - Token:" << Book::Enums::Code::endl << Token.Mark(B);

    return CheckEofStream(A);
}

std::string STMLText::Compiler::Mark()
{
    const char* Left = C;
    const char* Right = C;
    while ((Left >= B) && (*Left != '\n')) --Left;
    ++Left;
    while (Right && (*Right != '\n')) ++Right;
    --Right;

    std::string MStr = std::string(Right - Left, ' ');

    MStr += Utf::Glyph::CArrowUp;
    StrAcc Str;
    Str << Color::Grey78 << std::string(Left, Right - Left) << '\n' << Color::Yellow << MStr << Color::Reset;
    return Str();
}






Book::Enums::Code STMLText::Compiler::CompileColor(STMLText::STMLAttribute& A)
{

    auto Token = STMLText::STMLToken::ScanToToken(C);

    if (Token.M != STMLText::STMLToken::STMLMnemonic::ArgSeq)
    {
        AppBook::Syntax() << Book::Enums::Code::Unexpected << Book::Enums::Fn::Endl << Token.Mark(B);
        return Book::Enums::Code::Unexpected;
    }

    EatToken(Token);

    Token = ScanIdentifier();
    if(!Token)
    {
        AppBook::Syntax() << Book::Enums::Code::Expected << " Expected Identifier token." << Book::Enums::Fn::Endl << Mark();
        return Book::Enums::Code::Expected;
    }

    auto cid = ColorID(Token);
    if (cid == Color::Reset) return Book::Enums::Code::Accepted;

    A.FG = cid;
    A._Assigned.f = 1;
    // Ici on doit verfifer si on a une virgule ou eos ou closing tag;
    // result "," | ';' | '>'.

    Token = STMLText::STMLToken::ScanToToken(C);
    if ((Token.M != STMLText::STMLToken::STMLMnemonic::ArgSep) && (Token.M != STMLText::STMLToken::STMLMnemonic::Eos) && (Token.M != STMLText::STMLToken::STMLMnemonic::ClosingTag))
    {
        // Book::Enums::Code::push_syntax() << Book::Enums::Code::expected << "Expected ',' (arg separator) or eos (';') or closing tag ('>') " << Book::Enums::Code::endl << Token.Mark(B);
        return Book::Enums::Code::Rejected;
    }
    if ((Token.M == STMLText::STMLToken::STMLMnemonic::Eos) || (Token.M == STMLText::STMLToken::STMLMnemonic::ClosingTag))
    {
        if (A.FG == Color::Reset)
        {
            A.BG = A.FG;
            if(Token.M == STMLText::STMLToken::STMLMnemonic::Eos)
                EatToken(Token);

            return Book::Enums::Code::Accepted;
        }

    }
    // Ici on a obligatoirement argsep:

    EatToken(Token);

    Token = ScanIdentifier();
    //...
    cid = ColorID(Token);
    A.BG = cid;
    A._Assigned.b = {1};
    return CheckEofStream(A);
}


/**
@brief not implement yet

@param A
@return Book::Enums::Code
 */
Book::Enums::Code STMLText::Compiler::CompileBRKLine(STMLText::STMLAttribute& A)
{
    AppBook::Warning() << " 'line-break' ( <br> ) are not handled as of now.";

    return Book::Enums::Code::Notimplemented;
}



/**
@brief not implemented yet

@return Book::Enums::Code::Accepted
*/
Book::Enums::Code STMLText::Compiler::CloseAttribute(STMLText::STMLAttribute &A)
{
    //TextRef.PushAttribute(A);
    return Book::Enums::Code::Accepted;
}

Book::Enums::Code STMLText::Compiler::CheckEofStream(STMLText::STMLAttribute& A)
{
    //Skip();
    auto Token = STMLText::STMLToken::ScanToToken(C);
    if ( (!Token) || ((Token.M != STMLText::STMLToken::STMLMnemonic::Eos) && (Token.M != STMLText::STMLToken::STMLMnemonic::ClosingTag)))
    {
        AppBook::Warning() << Book::Enums::Code::Expected << ":" << Book::Enums::Fn::Endl << Token.Mark(B);
        return Book::Enums::Code::Rejected;
    }
    if (Token.M == STMLText::STMLToken::STMLMnemonic::ClosingTag)
    {

        return Book::Enums::Code::Accepted;
    }
    EatToken(Token);
    A.End = Token._Location.End;
    return Book::Enums::Code::Accepted;
}


STMLText::STMLToken STMLText::Compiler::ScanIdentifier()
{
    const char* Sc = C;
    while (isspace(*Sc)) ++Sc;
    C = Sc;
    if (!isalpha(*Sc) && (*Sc != '_'))
    {
        AppBook::Syntax() << Book::Enums::Fn::Function << ": " << Book::Enums::Code::Expected << " Identifier, got " << *Sc << " instead." << Book::Enums::Fn::Endl << Mark();
        return {};
    }

    STMLText::STMLToken Token;
    while (*Sc && (isalnum(*Sc) || (*Sc == '_'))) ++Sc;
    --Sc;
    Token._Location.Begin = C;
    Token._Location.End = Sc;
    Token.L = C;
    Token.T = STMLToken::Type::Identifier;
    return Token;
}

Book::Enums::Code STMLText::Compiler::EatToken(STMLText::STMLToken& Token)
{
    C = Token._Location.End;
    C++;
    //Book::debug() << (int)(*C) << " Cursor on '" << Color::Yellow << *C << Color::Reset << '\'';

    return Book::Enums::Code::Accepted;
}


Color::Code STMLText::Compiler::ColorID(STMLToken& Token)
{
    if(std::string(Token.L) == STMLText::STMLToken::Reset)
    {
        EatToken(Token);
        return Color::Reset;
    }

    auto Str = Token();
    Color::Code Colr = Color::Scan(Str);
    if (Colr == Color::Reset)
    {
        if (Str != "Reset")
        {
            AppBook::Error() << " Expected Color::Code name (strict case match). Got '" << Color::Yellow << Str << Color::White << "' instead:" << Book::Enums::Fn::Endl << Token.Mark(B);
            return Color::Reset;
        }
    }
    EatToken(Token);
    return Colr;
}



Utf::Glyph::Type STMLText::Compiler::GlyphID(STMLToken& Token)
{
    auto Str = Token();
    Utf::Glyph::Type IconId = Utf::Glyph::Scan(Str);
    if (IconId == 0)
    {
        AppBook::Error() << " Expected Utf::Glyph::Type name, got '" << Color::Yellow << Str << Color::White << "' insbookd:" << Book::Enums::Fn::Endl << Token.Mark(B);
        return 0;
    }
    EatToken(Token);
    return IconId;
}

STMLText::STMLAttribute::STMLAttribute(STMLToken& aInfo) : Begin(aInfo._Location.Begin - 1), End(aInfo._Location.End), Curs(nullptr) {}

std::string STMLText::STMLAttribute::operator()() const
{
    if (End)
    {
        std::string Str{Begin, size_t(End - Begin) + 1 };
        return Str;
    }

    AppBook::Warning() << Book::Enums::Fn::Function << " : Incomplete ( unclosed ) STMLAttribute data : ";
    std::string Str{Begin };
    return Str;

}

std::string STMLText::STMLAttribute::Infos()
{
    return "implement";
}



}
