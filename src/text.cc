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

#include <logbook/text.h>
#include <map>
#include <logbook/Book.h>



namespace book
{


//--------------------------------------------------------------------------------------------------------------------------------------------


// Referentiel des tokens du processeur :
text::token_data::list text::token_data::Referential =
{
    {text::token_data::type::Punctuation,    text::token_data::mnemonic::OpenTag,     text::token_data::OpenSeq, 0},
    {text::token_data::type::Punctuation,    text::token_data::mnemonic::ClosingTag,  text::token_data::CloseSeq,0},
    {text::token_data::type::Punctuation,    text::token_data::mnemonic::ArgSeq,      text::token_data::ArgSeq,0},
    {text::token_data::type::Punctuation,    text::token_data::mnemonic::ArgSep,      text::token_data::ArgSep,0},
    {text::token_data::type::Punctuation,    text::token_data::mnemonic::Eos,         text::token_data::EoSt,0},
    {text::token_data::type::Punctuation,    text::token_data::mnemonic::Reset,       text::token_data::Reset,0},
    {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Color,       text::token_data::ColorPair,2},
    {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Fg,          text::token_data::Fg,1},
    {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Bg,          text::token_data::Bg,1},
    {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Ic,          text::token_data::Icon,1},
    {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Br,          text::token_data::LineBreak,0},
    {text::token_data::type::Accent,         text::token_data::mnemonic::AccentSeq,   text::token_data::Accent,1}
    //...
};

// Parsers:




text::text()
{

}

text::text(const text& other)
{
    _d = other._d;
}

text::text(const std::string& Str) :_d(Str) {}

text::text(std::string&& Str) noexcept : _d(std::move(Str)) {}

text::text(const char* aStr) : _d(aStr) {}

text::~text()
{
    _d.clear();
    _attributes.clear();
}



text::token_data text::token_data::scan(const char* Start)
{
    ////Book::debug(HERE) << " Entering with Start :[" << *Start << "] -> '" << Start << "':";
    if (!*Start)
        return {};

    for (auto TokenRef : text::token_data::Referential)
    {
        TokenRef._location.s_begin = Start;
        while (isspace(*Start)) ++Start;

        auto crs = Start;
        const char* rtxt = TokenRef.L;
        //auto sz = std::strlen(rtxt);

        if (toupper(*crs) != toupper(*rtxt)) continue;

        while (*rtxt && *crs && (toupper(*crs) == toupper(*rtxt))) { ++crs; ++rtxt; }
        ////Book::debug(HERE) << "Token.L = [" << color::Yellow << TokenRef.L << color::Reset << "]:";
        if (!*rtxt)
        {   // fin de Token.L :
            if (TokenRef.T == token_data::type::AttrCmd)
            {
                // Si le mot continu alors ce n'est pas un Token:
                if (*crs)
                {   // *crs n'est pas un type d'espace:
                    if (isalnum(*crs))
                        continue;
                }
            } // Il reste les autres non-espace comme les ponctuations, symboles...
            --crs; // Replacer crs sur le dernier caractere du Token.
            //// book::code::push_debug(HERE) << " Scanned to :'" << color::Yellow << *crs << color::Reset << '\'';
            TokenRef._location.begin = Start;
            TokenRef._location.end = crs; // Fin du Token

            return TokenRef; // On le retourne.... doh!
        }
    }
    // book::code::push_debug(HERE) << " '" << *Start << "' not a TokenRef";
    return {}; // crs ( ou Start )  n'est pas sur un token du referentiel.
}


text::token_data::token_data(text::token_data::type aType, text::token_data::mnemonic aMnenomic, text::token_data::Lexem aLexem, int nParams) :
    T(aType), M(aMnenomic), L(aLexem),NParam(nParams)
{

}

text::token_data::token_data(text::token_data&& r) noexcept :
    T(r.T), M(r.M), L(r.L)
{

    _location = r._location;
    //book::code::Debug(SourceLocation) << " ->[" << (*this)() << "]";
}

std::string text::token_data::mark(const char* Stream)
{
    if (!*this)
        return " - Can't mark an invalid token...";
    const char* B = _location.begin;
    const char* E = _location.end;
    while ((B >= Stream) && (*B != '\n')) --B;
    ++B;
    while (*E && (*E != '\n')) ++E;
    if(!E) --E;

    std::string MStr = std::string(_location.begin - Stream, ' ');
    MStr += Icon::CArrowUp;
    stracc Str;
    Str << color::Grey78 << std::string(B, E - B) << '\n' << color::Yellow << MStr << color::Reset;
    return Str();
}

book::code text::compile()
{
    ////Book::debug(HERE) << " :";
    text::compiler Parse{ *this };
    return Parse.execute();
}

book::code text::operator>>(std::string& Out)
{
    ////Book::debug(HERE) << " Building the text using the processed data:";
    if(state == text::err)
    {
        Book::error(HERE) << " Bad state error.";
        return book::code::failed;
    }

    const char* r = _d.c_str();
    auto l = _d.length();
    const char* b = r;

    std::string AttrStr;

    for (auto const& A : _attributes)
    {
        //Book::debug(HERE) << " Looping Attributes: '" << A() << "' length:" << A().length();
        while (r != A._begin)
        {
            Out += *r;
            if(!output_hook.empty())
            {
                auto R = output_hook(*r);
                if(*R != book::code::accepted) return *R;
            }
            r++;
        }
        r += A().length();
        if(!attr_hook.empty())
        {
            auto R = attr_hook(A);
            if(*R != book::code::accepted) return *R;
        }

        // ----------- Foreground and background assign colors: -----------------------------
        if(A._assign.foreground && A._assign.background)
        {
            chattr::pair{A._fg, A._bg} >> AttrStr;
            Out += AttrStr;
        }
        else
        {

            if (A._assign.background)
            {
                AttrStr = _f == chattr::format::ansi256 ? attr<chattr::format::ansi256>::bg(A._bg) : attr<chattr::format::html>::bg(A._bg);
                Out += AttrStr;
            }
            else
            if (A._assign.foreground)
            {
                //Book::debug(HERE) << " foreground color #: " << static_cast<int>(A._fg);
                AttrStr = _f == chattr::format::ansi256 ? attr<chattr::format::ansi256>::fg(A._fg) : attr<chattr::format::html>::fg(A._fg);
                Out += AttrStr;
            }
        }
        // ----------- Foreground and background assign colors end -----------------------------

        // ----------- Graphens and Accents: -----------------------------
        if (A._assign.accent)
        {
            AttrStr = Accent::Data[A._accnt];
            Out += AttrStr;
        }
        else
        {
            if (A._assign.icon)
            {
                AttrStr = Icon::Data[A._icn];
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
        output_hook(*r);
        Out += *r++;
    }
    return book::code::success;
}

book::code text::operator()()
{
    if(output_hook.empty() || attr_hook.empty() ) return book::code::rejected;
    if(compile() != book::code::accepted) return book::code::rejected;

    const char* r = _d.c_str();
    auto l = _d.length();
    const char* b = r;

    std::string AttrStr;

    for (auto const& A : _attributes)
    {
        while (r != A._begin)
        {
            auto R = output_hook(*r);
            if(!R)
                return R();
            r++;
        }
        r += A().length();
        auto R = attr_hook(A);
        if(!R)
            return R();
    }
    while (static_cast<size_t>(r - b) < l) output_hook(*r++);

    return book::code::accepted;
}

book::text& text::operator<<(const std::string& input_str)
{
    clear();
    _d = input_str;
    //Book::debug(HERE) << " calling text::compile():";
    auto c = compile();
    if(c != book::code::accepted)
    {
        Book::error(HEREF) << c;
        state = text::err;
    }
    return *this;
}

void text::clear()
{
    _d.clear();
    _attributes.clear();
}

void text::push_attribute(text::attribute Attr)
{
    _attributes.push_back(Attr);
}

text::token_data text::scan(text::token_data::mnemonic M)
{
    for (auto iM : text::token_data::Referential)
    {
        if (iM.M == M) return iM;
    }
    return {};
}


//     text::Attribute::Attribute(const char* Start)
//     {
//         Begin = C = End = Start;
//     }

std::string text::token_data::operator()()
{
    if (L == nullptr)
        return { "null" };

    return { _location.begin, (std::size_t)(_location.end - _location.begin) + 1 };
}

text::compiler::compiler(text& aTextInstance) :
    B(aTextInstance._d.c_str()), C(aTextInstance._d.c_str()), E(aTextInstance._d.c_str()), TextRef(aTextInstance)
{
    E = B + aTextInstance._d.length() - 1;
}

bool text::compiler::operator ++()
{
    return false;
}

bool text::compiler::operator ++(int)
{
    return false;
}

//book::code text::Compiler::Skip()
//{
//    //book::code::Debug() << " text::TextParser::Skip(" << color::Yellow << *C << color::Reset << "):";
//    while (C <= E)
//    {
//        ++C;
//        if (!isspace(*C))
//            return book::code::book::code::Accepted;
//    }
//    return book::code::book::code::Eof;
//}



void text::compiler::close_token(token_data& Info)
{
    Info._location.index = static_cast<int>(Info._location.end - B);
    auto sz = Info._location.end - Info._location.begin;
    C += sz + 1;

    // book::code::push_debug({}) << " Token :" << book::code::endl << Info.mark(B);

}

text::token_data text::compiler::scan()
{
    auto Token = text::token_data::scan(C);
    if (!Token)
    {
        auto R = scan_identifier();
        if (!R)
        {
            // book::code::push_syntax({}) << " In text::compiler::scan :" << book::code::endl << Token.mark(TextRef._d.c_str());
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
book::code text::compiler::execute()
{
    //Book::debug(HERE) << ":";
    while (!eof())
    {

        auto Token = skip_to_attribute();
        text::attribute Attr;
        text::attribute A;
        if (!Token) return book::code::accepted;
        if (Token.M == text::token_data::mnemonic::AccentSeq)
        {
            Attr._begin = Token._location.begin;
            A = compile_accent(Attr);
        }
        else if (Token.M == text::token_data::mnemonic::OpenTag)
        {
            Attr._begin = Token._location.begin;
            A = compile_attribute(Attr);
        }

        if (!A)
        {
            // book::code::push_debug() << " No attribute. Skipping to next \"OpenTag | AccentSeq\"...";
            //Skip();
            continue;
        }
        TextRef.push_attribute(Attr);

    }
    return book::code::accepted;
}

/*!
    @brief Compiler une sequence d'attribut(s).

    Ici c'est moins &eacute;vident... On d&eacute;coupe ce qui est entre '<' et '>' en commandes d'attribut.
    //...
    // '<' 'Fg' ':' 'ColorID' ';'...'Color' : 'ColorID(Fg)' ',' 'ColorID(Bg)' ';' 'Reset' ';' '>'

    @note  "Lexing And Parsing... tout &ccedile;a en m&ecirc;me temps!".
*/
text::attribute text::compiler::compile_attribute(text::attribute& Attr)
{

    text::token_data Token;
    // On tokenize les elements d'attribut: ( Stop: ClosingTag('>') Token )
    std::map<text::token_data::mnemonic, text::compiler::parser_fnptr_t> Parsers =
    {
        {text::token_data::mnemonic::Fg, &text::compiler::parse_fg},
        {text::token_data::mnemonic::Bg, &text::compiler::ParseBg},
        {text::token_data::mnemonic::Color, &text::compiler::parse_color},
        {text::token_data::mnemonic::Ic, &text::compiler::parse_icon},
        //... Plus tard, on aura les Grilles et Fenetres
        {text::token_data::mnemonic::ClosingTag, &text::compiler::close_attribute},
        {text::token_data::mnemonic::Br, &text::compiler::parse_br},
    };
    while (!eof())
    {
        // on passe '<'
        //Skip();
        expect<> ER;
        // Expecting text::token_data::mnemonic:
        Token = text::token_data::scan(C);
        if (!Token)
        {
            Book::syntax(HERE) << " Expected ACM (Attribute Command mnemonic) Token - Returning empty Attribute.";
            return {};
        }
        // Ici c'est obligatoire de faire une boucle qui teste explicitement les mnemonics sp&eacute;cifiques
        // parce qu'on ne prend en charge tous les mnemonics.
        for (auto [M, Fn] : Parsers)
        {
            if (M != Token.M) continue;
            eat_token(Token);
            ER = (this->*Fn)(Attr);
            if (!ER) return {};
            ////Book::debug(HERE) << " Token : '" << Token.L  << "' - attribute-contruct[" << Attr() << color::White << "]";
            break;
        }

        if (Token.M == text::token_data::mnemonic::ClosingTag)
        {
            //eat_token(Token);
            Attr._end = Token._location.end;
            return Attr; // Peut &ecirc;tre vide si on es sur "<>"
        }
    }
    Book::syntax(HERE) << " Unexpected end of stream in Attribute parsing - Returning empty Attribute";
    return {};
}



/*!
    @brief Cr&eacute;&eacute; un Attribute de texte Accent.

    @note Encoder un Accent est le plus facile, donc pas besoin d'accumuler les tokens ici: juste "parser" '&' + Id + ';' c'est tout!
*/
text::attribute text::compiler::compile_accent(text::attribute& Attr)
{

    text::token_data Token;
    //Skip();
    if(eof())
    {
        // book::code::push_syntax() << "text::compiler::compile_accent :  Expected identifier.";
        return {};
    }

    Token = scan_identifier();
    if (!Token)
    {
        // book::code::push_syntax() << " Expected identifier." << book::code::endl << mark();
    }

    // book::code::push_debug() << " Identifier: " << Token();

    Accent::Type T = Accent::Code(Token());
    if (T == Accent::Err)
    {
        // book::code::push_syntax() << " Unknown Code token(identifier) " << book::code::endl << Token.mark(B);
        return {};
    }


    Attr = text::attribute(Token);
    Attr._accnt = T;
    Attr._assign.accent = 1;
    //Mandatory expect ';'
    (void)eat_token(Token);
    if(check_eos(Attr) != book::code::accepted) return Attr;
    // book::code::push_syntax(HERE) << " Expected Eos ';' (End Of Statement token)." << book::code::endl << mark();
    return {};
}

/*!
    @brief Tokenize jusqu'&agrave; '>' ou ';', puis compiler l'attribut.
*/

text::token_data text::compiler::skip_to_attribute()
{
    text::token_data Token;

    while (!eof())
    {
        auto c = toupper(*C);
        if (c != token_data::OpenSeq[0])
        {
            if (c != token_data::Accent[0])
            {
                ++C;
                continue;
            }
            Token.T = text::token_data::type::Accent;
            Token.L = text::token_data::Accent;
            Token.M = text::token_data::mnemonic::AccentSeq;
        }
        else
        {
            Token.T = text::token_data::type::AttrCmd;
            Token.L = text::token_data::OpenSeq;
            Token.M = text::token_data::mnemonic::OpenTag;
        }
        Token._location.begin = Token._location.end = C;
        Token._location.index = static_cast<int>(C - TextRef._d.c_str());
        eat_token(Token);
        return Token;
    }
    return {};
}

bool text::compiler::eof() { return C > E; }

book::code text::compiler::parse_icon(text::attribute& A)
{
   auto Token = text::token_data::scan(C);
   if (Token.T != text::token_data::type::Punctuation)
   {
       // book::code::push_syntax(HERE) << " Expected Punctuation token ':'" << book::code::endl << mark();
       return book::code::rejected;
   }
   eat_token(Token);
   Token = scan_identifier();
    auto R = icon_id(Token);
    if (R == 0)
    {
        // book::code::push_syntax({}) << book::code::notexist << " Icon ID:\n" << book::code::endl << Token.mark(B);
       return book::code::rejected;
    }
    A._icn = R;
    A._assign.icon = 1;
    return check_eos(A);
}



book::code text::compiler::parse_fg(text::attribute& A)
{
    // C sur 'Fg'; ( Consomm&eacute; )
    // Attendus :  ':' , 'ColorID', '; | >';

    auto Token = text::token_data::scan(C);
    //Book::debug(HERE) << Token();
    if(Token.L == text::token_data::Reset)
    {
       //Book::debug(HERE) << " Explicit assign fg:Reset to attribute.";
       eat_token(Token);
       A._assign.foreground = 1;
       A._fg = color::Reset; return book::code::accepted;
    }

    if ((Token.T != token_data::type::Punctuation) || (Token.L != text::token_data::ArgSeq))
    {
        Book::syntax(HERE) << " Expected token ':' " << book::functions::endl << mark();
        return book::code::unexpected;
    }
    if(Token.L == text::token_data::Reset)
    {
        //Book::debug(HERE) << "Reset: check...";
        eat_token(Token);
        return book::code::accepted;
    }

    eat_token(Token);
    Token = scan_identifier();
    if (!Token)
    {
        Book::syntax(HERE) << " Expected Identifier token." << book::functions::endl << mark();
        return book::code::expected;
    }

    auto cid = color_id(Token);
    if (cid==color::Reset ) return book::code::accepted;
    A._fg = cid;
    A._assign.foreground = 1;
    //Book::out() << " Compiler::ParseFg - Token:" << book::functions::endl << Token.mark(B);
    return check_eos(A);
}

book::code text::compiler::ParseBg(text::attribute& A)
{
    // C sur 'Fg'; ( Consomm&eacute; )
    // Attendus :  ':' , 'ColorID', '; | >';
    auto Token = text::token_data::scan(C);
    if(Token.L == text::token_data::Reset)
    {
        //Book::debug(HERE) << " Explicit assign fg:Reset to attribute.";
        eat_token(Token);
        A._assign.background = 1;
        A._bg = color::Reset;
        return book::code::accepted;
    }

    if ((Token.T != token_data::type::Punctuation) || (Token.L != text::token_data::ArgSeq))
    {
        // book::code::push_syntax({}) << book::code::rejected << " Expected token ':'" << book::code::endl << mark();
        return book::code::rejected;
    }
    eat_token(Token);
    Token = scan_identifier();
    if (!Token)
    {
        // book::code::push_syntax({}) << book::code::rejected << " Expected Identifier token." << book::code::endl << mark();
        return book::code::rejected;
    }

    auto cid = color_id(Token);
    if (cid == color::Reset) return book::code::accepted;
    A._bg = cid;
    A._assign.background = 1;
    //book::code::out() << " Compiler::ParseFg - Token:" << book::code::endl << Token.mark(B);

    return check_eos(A);
}

std::string text::compiler::mark()
{
   const char* Left = C;
   const char* Right = C;
   while ((Left >= B) && (*Left != '\n')) --Left;
   ++Left;
   while (Right && (*Right != '\n')) ++Right;
   --Right;

   std::string MStr = std::string(Right - Left, ' ');

   MStr += Icon::CArrowUp;
   stracc Str;
   Str << color::Grey78 << std::string(Left, Right - Left) << '\n' << color::Yellow << MStr << color::Reset;
   return Str();
}






book::code text::compiler::parse_color(text::attribute& A)
{

    auto Token = text::token_data::scan(C);

    if (Token.M != text::token_data::mnemonic::ArgSeq)
    {
        Book::syntax(HERE) << book::code::unexpected << book::functions::endl << Token.mark(B);
        return book::code::unexpected;
    }

    eat_token(Token);

    Token = scan_identifier();
    if(!Token)
    {
        Book::syntax(HERE) << book::code::expected << " Expected Identifier token." << book::functions::endl << mark();
        return book::code::expected;
    }

    auto cid = color_id(Token);
    if (cid==color::Reset) return book::code::accepted;

    A._fg = cid;
    A._assign.foreground = 1;
    // Ici on doit verfifer si on a une virgule ou eos ou closing tag;
    // result "," | ';' | '>'.

    Token = text::token_data::scan(C);
    if ((Token.M != text::token_data::mnemonic::ArgSep) && (Token.M != text::token_data::mnemonic::Eos) && (Token.M != text::token_data::mnemonic::ClosingTag))
    {
        // book::code::push_syntax() << book::code::expected << "Expected ',' (arg separator) or eos (';') or closing tag ('>') " << book::code::endl << Token.mark(B);
        return book::code::rejected;
    }
    if ((Token.M == text::token_data::mnemonic::Eos) || (Token.M == text::token_data::mnemonic::ClosingTag))
    {
        if (A._fg == color::Reset)
        {
            A._bg = A._fg;
            if(Token.M == text::token_data::mnemonic::Eos)
                eat_token(Token);

            return book::code::accepted;
        }

    }
    // Ici on a obligatoirement argsep:

    eat_token(Token);

    Token = scan_identifier();
    //...
    cid = color_id(Token);
    A._bg = cid;
    A._assign.background = {1};
    return check_eos(A);
}


/**
@brief not implement yet

@param A
@return book::code
 */
book::code text::compiler::parse_br(text::attribute& A)
{
    Book::warning(HERE) << " 'line-break' ( <br> ) are not handled as of now.";

    return book::code::notimplemented;
}



/**
@brief not implemented yet

@return book::code::accepted
*/
book::code text::compiler::close_attribute(text::attribute& )
{
    //TextRef.PushAttribute(A);
    return book::code::accepted;
}

book::code text::compiler::check_eos(text::attribute& A)
{
    //Skip();
    auto Token = text::token_data::scan(C);
    if ( (!Token) || ((Token.M != text::token_data::mnemonic::Eos) && (Token.M != text::token_data::mnemonic::ClosingTag)))
    {
        Book::warning(HERE) << book::code::expected << ":" << book::functions::endl << Token.mark(B);
        return book::code::rejected;
    }
    if (Token.M == text::token_data::mnemonic::ClosingTag)
    {

        return book::code::accepted;
    }
    eat_token(Token);
    A._end = Token._location.end;
    return book::code::accepted;
}


text::token_data text::compiler::scan_identifier()
{
    const char* Sc = C;
    while (isspace(*Sc)) ++Sc;
    C = Sc;
    if (!isalpha(*Sc) && (*Sc != '_'))
    {
        Book::syntax() << book::functions::function << ": " << book::code::expected << " Identifier, got " << *Sc << " insbookd." << book::functions::endl << mark();
        return {};
    }

    text::token_data Token;
    while (*Sc && (isalnum(*Sc) || (*Sc == '_'))) ++Sc;
    --Sc;
    Token._location.begin = C;
    Token._location.end = Sc;
    Token.L = C;
    Token.T = token_data::type::Identifier;
    return Token;
}

book::code text::compiler::eat_token(text::token_data& Token)
{
    C = Token._location.end;
    C++;
    //Book::debug(HERE) << (int)(*C) << " Cursor on '" << color::Yellow << *C << color::Reset << '\'';

    return book::code::accepted;
}


color::code text::compiler::color_id(token_data& Token)
{
    if(std::string(Token.L)  == text::token_data::Reset)
    {
        eat_token(Token);
        return color::Reset;
    }

    auto Str = Token();
    color::code Colr = chattr::scan(Str);
    if (Colr == color::Reset)
    {
        if (Str != "Reset")
        {
            Book::error(HERE) << " Expected color::code name (strict case match). Got '" << color::Yellow << Str << color::White << "' insbookd:" << book::functions::endl <<Token.mark(B);
            return color::Reset;
        }
    }
    eat_token(Token);
    return Colr;
}



Icon::Type text::compiler::icon_id(token_data& Token)
{
    auto Str = Token();
    Icon::Type IconId = Icon::Scan(Str);
    if (IconId == 0)
    {
        Book::error(HERE) << " Expected Icon::type name, got '" << color::Yellow << Str << color::White << "' insbookd:" << book::functions::endl << Token.mark(B);
        return 0;
    }
    eat_token(Token);
    return IconId;
}

text::attribute::attribute(token_data& aInfo) : _begin(aInfo._location.begin-1), _end(aInfo._location.end), _crs(nullptr) {}

std::string text::attribute::operator()() const
{
    if (_end)
    {
        std::string Str{ _begin, size_t(_end - _begin) + 1 };
        return Str;
    }

    Book::warning() << book::functions::function << " : Incomplete ( unclosed ) attribute data : ";
    std::string Str{ _begin };
    return Str;

}

std::string text::attribute::informations()
{
    return "implement";
}



}
