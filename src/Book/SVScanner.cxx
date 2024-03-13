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


#include "AppBook/Book/SVScanner.h"
//using namespace std::literals;
#include <AppBook/Utf/Glyphes.h>
namespace Book
{

SVScanner SVScanner::Numeric::Empty{};
[[maybe_unused]] std::string_view SVScanner::Word::_Separators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";

SVScanner::Word &SVScanner::Word::operator<<(const SVScanner::LocationData &LD)
{

    return *this;
}

std::string_view SVScanner::Word::operator()() const
{
    if(Begin == End) return {};
    return {Begin, End};
}


SVScanner::SVScanner(std::string_view Txt) : Text(Txt)
{
    mBegin = mPos = Text.begin();
    mEnd = Text.end();
    mLocation = {1, 1, 0};

}

bool SVScanner::PopLocation()
{
    if(mPoints.empty()) return false;
    auto P = --mPoints.end();
    mLocation.Line = P->first;
    mLocation.Col = P->second;
    mPoints.erase(P);
    return true;
}
void SVScanner::PushLocation()
{
    mPoints.emplace_back(mLocation.Line, mLocation.Col);

}

bool SVScanner::Eof(std::string_view::iterator cc)
{
    return cc >= mEnd;
}


bool SVScanner::Eof()
{
    return mPos >= mEnd;
}


bool SVScanner::SkipWS()
{
    //AppBook::Debug();
    while (!Eof() && std::isspace(*mPos)) ++mPos;
    //AppBook::Debug() << " Iterator now on '" << Color::Yellow << *mPos << Color::Reset << "' :";
    return Eof();
}

/*!
 * @brief Synchronize internal "cursor" location {Line #, Column #, Offset} at the current iterator offset into the view.
 * @return  Reference to the updated infos into the LocationData member of TextProc.
 */
SVScanner::LocationData &SVScanner::Sync()
{
    auto c = mBegin;
    //AppBook::Debug() << "'" << std::string_view{mPos,mEnd} << "'" << Book::Fn::Endl << "mBegin -> c: " << std::string_view{c,mPos};
    while (!Eof(c) && (c < mPos)) {
        switch (*c) {
            case '\n':
                //AppBook::Debug() << " new line :";
                ++c;
                if (*c == '\r') ++c;
                ++mLocation.Line;
                mLocation.Col = 1;
                continue;
            case '\r':
                //AppBook::Debug() << " new line :";
                ++c;
                if (*c == '\n') ++c;
                ++mLocation.Line;
                mLocation.Col = 1;
                continue;
            case '\t':
            case '\v' :
                throw AppBook::Exception()[AppBook::Error() << Result::Rejected << " - Invalid character, intentionally not handled in this context."];

            default:
                ++c;
                ++mLocation.Col;
        }
    }
    mLocation.Offset = mPos - mBegin;
    // ...
    AppBook::Debug() << " Done";
    return mLocation;
}

SVScanner::operator bool() const
{
    return !Text.empty();
}


/*!
 * @brief Initiate this TextProc with a (new) string_view.
 * @param view
 * @return reference to this instance.
 */
SVScanner &SVScanner::operator=(std::string_view view)
{
    Text = view;
    if(view.empty())
    {
        AppBook::Error() << "TextProc assigned with an empty string \"view\". Be aware, this instance cannot be used.";
        return *this;
    }
    mBegin = mPos = Text.begin();
    mEnd = Text.end();
    mLocation = {1, 1, 0};

    return *this;
}



bool SVScanner::operator++()
{
    if(Eof()) return false;

    ++mPos;
    SkipWS();
    return Eof();
}

bool SVScanner::operator++(int)
{
    if(Eof()) return false;

    ++mPos;
    SkipWS();
    return Eof();
}

Book::Result SVScanner::Seek(int32_t Idx)
{
//    if(Text.empty()) return Book::Result::Rejected;

//    Signal Exception on any attempt to seek into an empty string_view !! :
    if(Text.empty())
        AppBook::Exception() [ AppBook::Except() << Book::Result::Empty << ": Attempt to use the Seek method on an empty SVScanner!" ];

    // Otherwise just reject under/overflow :
    // Otherwise execute the seek and return Accepted.
    if(Idx < 0L)
    {
        size_t X =  -1 * Idx;
        if(X >= Text.size()) return Book::Result::Rejected;
        mPos  = Text.end() - X;
        return Book::Result ::Accepted;
    }
    if((mBegin + Idx) < mEnd ) mPos = mBegin + Idx;
    else
        mPos = mEnd-1; // Jump to the end of the text...

    Sync();

    return Eof() ? Book::Result::Eof : Book::Result::Accepted;
}

//
//void SVScanner::Seek(std::string_view::iterator IPos)
//{
//    mPos = IPos;
//}

SVScanner::Numeric::Result SVScanner::ScanNumber()
{
    Numeric Scanner(*this);
    if (!Scanner.Base2() && !Scanner.Base8() && !Scanner.Base16() && !Scanner.Base10()) return { Book::Result::Rejected, {} };

    return { Book::Result::Accepted, Scanner.NumDetails };
}


/*!
 * @brief Scan literal string between quotes
 * @return string_view of the contents including the surrounding quotes.
 */
std::pair<Book::Result, std::string_view> SVScanner::ScanLiteralString()
{
    StrAcc Buf;
    std::string_view::iterator A = mPos;
    if ((*A != '\'') && (*A != '"')) return { Book::Result::Rejected,{} };
    auto Q = *A++;

    // Ignore `""`  (empty string)
loop_str:
    do {

        if (*A == '\\')
        {
            AppBook::Debug() << " A on '" << Color::Yellow << *A << Color::Reset << "':";
            Buf << *A++;
            AppBook::Debug() << " A on '" << Color::Yellow << *A << Color::Reset << "':";
            Buf << *A++;
            AppBook::Debug() << " A on '" << Color::Yellow << *A << Color::Reset << "':";
        }
        else
            if (!Eof())
                Buf << *A++;
        if (*A == '"')
            Buf << *A;
    } while (!Eof() && (*A != Q));

    if(Eof())
        return { Book::Result::Rejected,{} };

    return { Book::Result::Accepted, {mPos, ++A} };
}

std::string SVScanner::Mark()
{
    // 1 - Locate Beginning of the line:
    auto LBegin = mPos;
    auto LEnd   = mPos;
    // Beginning of the line:
    while((LBegin > mBegin) && (*LBegin != '\n') && (*LBegin != '\r')) --LBegin;
    if(LBegin < mBegin) ++LBegin;
    // End of line:
    while((LEnd < mEnd) && (*LEnd != '\n') && (*LEnd != '\r')) ++LEnd;
    if(LEnd >= mEnd) --LEnd;
    auto pos = mPos-LBegin;
    auto spc = std::string(pos,' ');
    return std::string(LBegin, LEnd) + '\n' + spc + Utf::Glyph::CArrowUp;
}

SVScanner::Iterator SVScanner::Begin()
{
    return mBegin; /// << The start if this Scanner
}

SVScanner::Iterator SVScanner::End()
{
    return mEnd; //< the end of this scanner.
}

void SVScanner::Push()
{
    PStack.push(mPos);
}

bool SVScanner::Pop()
{
    if(PStack.empty()) return false;
    auto Pos = PStack.top();
    PStack.pop();
    return true;
}

Book::Result SVScanner::Reposition(int Offset)
{
    if(Eof()) return Book::Result::Rejected;
    mPos += Offset;
    return Result::Accepted;
}

SVScanner::Word::Result SVScanner::Words(SVScanner::Word::Opt Opt, std::string_view Delims)
{
    _Word_Options = Opt;
    _Words_Separators = Delims.empty() ? SVScanner::Word::_Separators : Delims;

    Word::Result Data= {Book::Result::Empty,{}};

    if(Empty())
    {
        AppBook::Warning() << Color::Yellow << " Scanning words on empty text segment";
        return Data;
    }

    SkipWS();
    Sync();   // Update Offset vars ( Line, Column, Offset, Iterators ).
    Word W;
    W.Begin = mPos;


    // Il faut à tout prix éviter de mettre des `quotes` dans la chaîne séparateur car ceux-ci sont exactement pris comme étant des ...`quotes` ...
    //---------------------------------------------------------------------------------------------------------------------------------------------
    while(!Eof())
    {
        if(_Words_Separators.find(*mPos) != std::string_view::npos)
        {
            // Ici on entre dans le traitement des séparateurs:
            // ---------------------------------------------------

            if(mPos > W.Begin) --mPos;
            // On fini et on push le 'mot' courant :
            Data.second.push_back({W.Begin, mPos++});
            //----------------------------------------------------

            // On repart sur un autre 'mot' :
            //--------------------------------
            W = {mPos,mPos};
            // '//' As one sequence instead of having two consecutive '/'
            if((*mPos == '/') && (*(mPos + 1) == '/')) ++mPos;

            // Faire du 'séparateur ou séquence de 'séparateurs contingues' un mot a insérer s'il est désiré de garder les séparateurs:
            if(Opt == Word::Opt::Keep)
                Data.second.push_back({W.Begin, mPos++});

            W = {mPos,mPos}; // On avance le nouveau mot avec mPos qui est passé au charactère suivant ...

            if(Eof())
            {
                // On oubli la dernier séquence de scan puisque la fin du text est atteinte:
                Data.first  = Book::Result::Success;
                return Data;
             };
        }
        //---------------------------------------------------------------------------------------------------------------------------------------------

        // mPos n'est pas sur un séparateur alors on passe à la vérification (analyse) suivante:
        // -------------------------------------------------------------------------------------
        else if((*mPos == '\'') || (*mPos == '"'))
        { // Quoted literal string...
            if(Opt == Word::Opt::Keep)
            {
                // Create the three parts of the quoted string: (") + (literal) + (") ( or ' )
                // So, we save the literal Sequence anyway.
                Data.second.push_back({W.Begin, mPos});
            }

            auto Seq = ScanLiteralString();//w.Begin + (data.o == StrBreak::Opt::Keep ? 0 : 1), *cursor.Pos); // W.B is the starting position, _Cursor.m is the quote delim.
            if(!Seq.first)
            {
                AppBook::Error() << " Failed to scan/tokenize string sequence.";
                Data.first = Book::Result::Failed;
                return Data;
            }
            auto I = Seq.second.begin();
            while(I < )

            while(cursor.Pos < p)
                ++cursor; // compute white spaces!!!

            if(data.o == StrBreak::Opt::Keep)
            {
                // then push the litteral that is inside the quotes.
                data.Words.push_back({w.Begin + 1, p - 1, cursor.End, w.Line, w.Column, w.Offset});
                //++_Cursor; // _Cursor now on the closing quote
                cursor >> w; // Litteral is done, update W.
                data.Words.push_back({w.Begin, p, cursor.End, w.Line, w.Column, w.Offset});
            }
            else
            {
                // PushLocation the entire quote delims surrounding the litteral As the token_t.
                data.Words.push_back({w.Begin, cursor.Pos, cursor.End, w.Line, w.Column, w.Offset});
            }
            if(++cursor)
                cursor >> w;
            else
                return data.Words.size();

        }
        else
        {
            cc = cursor.Pos;
            ++cc;
            if(cc == cursor.End)
            {
                ++cursor.Pos;
                break;
            }
            if(isspace(*cc))
            {
                if(w.Begin < cc)
                {
                    data.Words.push_back({w.Begin, cc - 1, cursor.End, w.Line, w.Column, w.Offset});
                    ++cursor;
                }

                if(cursor.SkipWS())
                {
                    cursor >> w;
                    continue;
                }
                return data.Words.size();
            }
            if(!cursor.AtEnd())
                ++cursor; // advance offset To the next separator/white space.
        }

    }
    if(cursor.Pos > w.Begin)
        data.Words.push_back({w.Begin, cursor.Pos - 1, cursor.End, w.Line, w.Column, w.Offset});

    return data.Words.size();



    return {};
}


SVScanner::LocationData const& SVScanner::LocationData::operator>>(std::string &Out) const
{
    StrAcc OutStr="%d,%d";
    OutStr << Line << Col;//  << Offset;
    Out = OutStr();
    return *this;
}

SVScanner::Numeric::Numeric(SVScanner &Tx):Text(Tx), End(Tx.mEnd), Pos(Tx.mPos), Begin(Tx.mPos){}

Book::Result SVScanner::Numeric::operator()()
{
    return Book::Result::Ok;
}

Book::Result SVScanner::Numeric::Base2()
{
    // 0b11010101010101010101010101010101010                --> Parsed all
    // 0b1101'0101'01010101'''''''0101'0101'0101'0101'0100'1010  --> Parsed all
    // 0b1101 0101 0101 0101 0101 0101 0101 0101 0100'1010  --> Parsed : 0b1101
    // 11010101'01010101'01010101'01010101'01001010B        --> Parsed : Rejected
    // 11010101 01010101 01010101 01010101 01001010         --> Parsed :  [base 10] 11'010'101


    auto A = Text();
    StrAcc Buf;
    NumDetails.Base = Details::BaseSize::Binary;

    if(std::toupper(*A) == 'B')
        Buf << *A++;
    else
    {
        if(*A == '0')
        {
            Buf << *A++;
            if (std::toupper(*A) == 'B')
                Buf << *A++;
            else
            {
                AppBook::Test() << " Rejected @Prefix '" << Color::Yellow << *A << Color::Reset << "'.";
                return Book::Result::Rejected;
            }
        }
    }
    if(A == Begin)
    {
        AppBook::Test() << " Rejected @Prefix '" << Color::Yellow << *A << Color::Reset << "'.";
        return Book::Result::Rejected;
    }

Loop2:
    AppBook::Debug() << "Loop2: A on '" << Color::Yellow << *A << Color::Reset << '\'';
    if(*A == '\'')++A;
    if(!isdigit(*A)) return Book::Result::Rejected;
    while(!Text.Eof() && std::isdigit(*A))
    {
        if(*A>='2') return Book::Result::Rejected;
        Buf << *A++;
    }
    if(!Text.Eof() && *A =='\'')
    {
        ++A;
        goto Loop2;
    }

    if(A==Begin) return Book::Result::Rejected;
    AppBook::Debug() << " Base2 : " << Color::Yellow << Buf << Book::Fn::Endl << " Length: " << Color::LightCoral << Seq.length();
    NumDetails.Seq = {Begin, A};
    NumDetails.Value = std::stoi(Buf(), nullptr, 2);
    NumDetails.Base = Details::BaseSize::Binary;
    NumDetails.ScaleValue();
    return Book::Result::Accepted;
}

Book::Result SVScanner::Numeric::Base8()
{
    auto A = Text();
    StrAcc Buf;
    NumDetails.Base = Details::BaseSize::Octal;

    std::string_view Prefixes = "oOq@$&";

    if(Prefixes.find(*A) != std::string_view::npos )
        A++;
    else
    {
        AppBook::Test() << " Rejected @Prefix '" << Color::Yellow << *A << Color::Reset << "'.";
        return Book::Result::Rejected;
    }

    loop8:
    AppBook::Debug() << "loop8: A on '" << Color::Yellow << *A << Color::Reset << '\'';
    while(!Text.Eof() && std::isdigit(*A) && (*A <= '7'))
    {
        Buf << *A++;
        //AppBook::Debug() << " Pushed digit'" <<  Color::Yellow << *(A-1) << Color::Reset << "' into the buffer. Next:'" << Color::Yellow << *A << Color::Reset << "'";
    }
    if(!Text.Eof() && (*A == '\''))
    {
        ++A;
        goto loop8;
    }
    AppBook::Debug() << "Octal loop exit: A on '" << Color::Yellow << *A << Color::Reset << '\'';

    if(((std::isdigit(*A) && (*A >= '7'))) || (A==Begin))
    {
        AppBook::Debug() << "Octal loop exit: A on '" << Color::Yellow << *A << Color::Reset << '\'' << Color::Red4 << " Rejected";
        return Book::Result::Rejected;
    }
    --A;
    AppBook::Debug() << "Base8: A on '" << Color::Yellow << *A << Color::Reset << "' - Buffer: [" << Color::Yellow << Buf << Color::Reset << "]";
    NumDetails.Seq = {Begin, A};
    NumDetails.Value = std::stoi(Buf(), nullptr, 8);
    NumDetails.Base = Details::BaseSize::Octal;
    NumDetails.ScaleValue();
    return Book::Result::Accepted;

}

Book::Result SVScanner::Numeric::Base10()
{
    AppBook::Debug() << " Base 2,8,16 rejected then:";
    auto A = Text(); // Get the current iterrator value...
    NumDetails.Base = Details::BaseSize::Decimal;

    StrAcc Buf;
    if((*A == '.') || (*A == ','))
    {
        Real = true;
        Buf << '.'; // Force '.' for convertion using StrAcc >> ;
        ++A;
    }
    AppBook::Debug() << "Decimal : A on '" << Color::Yellow << *A << Color::Reset << '\'';
    if(*A == '\'') ++A;
    AppBook::Debug() << "Decimal : A on '" << Color::Yellow << *A << Color::Reset << '\'';
    if(!std::isdigit(*A) ){
        //AppBook::Status() << " Rejected on '" << Color::Yellow << *A << Color::Reset << "'";
        return Book::Result::Rejected;
    }
    //AppBook::Debug() << "A on '" << Color::Yellow << *A << Color::Reset << '\'';

    while(!Text.Eof() && std::isdigit(*A))
    {
        //AppBook::Debug() << "While loop: A on '" << Color::Yellow << *A << Color::Reset << '\'';
        Buf << *A++;
        //AppBook::Debug() << "next to digit: A on '" << Color::Yellow << *A << Color::Reset << '\'';
        if(*A == '\''){++A; continue; }
        if((*A == '.') || (*A == ','))
        {
            if(Real)
            {
                //AppBook::Status() << " Rejected on '" << Color::Yellow << *A << Color::Reset << "' - Real flag already set.";
                return Book::Result::Rejected;
            }
            Real = true;
            Buf << '.';
            ++A;
            continue;
        }
        //AppBook::Debug() << "Bottom while loop: A on '" << Color::Yellow << *A << Color::Reset << '\'';
    }
    AppBook::Debug() << "A on '" << Color::Yellow << *A << Color::Reset << '\'';
    if(A == Begin)
    {
        AppBook::Status() << " Rejected on '" << Color::Yellow << *A << Color::Reset << "' Sequence is not a number.";
        return Book::Result::Rejected;
    }

    NumDetails.Seq = {Begin, A};
    NumDetails.Base = Details::BaseSize::Decimal;

    Buf >> NumDetails.Value;
    if(!Real)
        NumDetails.ScaleValue();
    else
        NumDetails.Size = Details::SizeType::F32;

    //AppBook::Debug() << " Base10 : " << Color::Yellow << Buf << Book::Fn::Endl << " Length: " << Color::LightCoral << Seq.length();

    return Book::Result::Accepted;
}

Book::Result SVScanner::Numeric::Base16()
{
    AppBook::Debug() << " Base 2,8 rejected - then:";
    auto A = Text();
    StrAcc Buf;
    NumDetails.Base = Details::BaseSize::Hexadecimal;

    if (*A == '0')
        ++A;

    if(std::toupper(*A) != 'X')
    {
        AppBook::Debug() << "Hex [Mandatory prefix not met]: " << Color::Red4 << "Rejected";
        return Book::Result::Rejected;
    }
    ++A;
loop16:
    AppBook::Debug() << "loop16: A on '" << Color::Yellow << *A << Color::Reset << '\'';
    while(!Text.Eof() && std::isxdigit(*A))
    {
        Buf << *A++;

    }
    if(!Text.Eof() && (*A == '\'')){ ++A; goto loop16; }

    if(A==Begin) return Book::Result::Rejected;
    AppBook::Debug() << "A on '" << Color::Yellow << *A << Color::Reset << '\'';
    NumDetails.Seq = {Begin, A};
    AppBook::Debug() << "[Hex] : Sequence ["
    << Color::Yellow << std::string(NumDetails.Seq.begin(), NumDetails.Seq.end())
    << Color::Reset << "] Length: "
    << Color::LightCoral << NumDetails.Seq.length()
    << Color::White <<  " -> Buffer:["
    << Color::Yellow << Buf
    << Color::Reset << "] ";

    NumDetails.Value = std::stoi(Buf(), nullptr, 16);
    NumDetails.ScaleValue();
    return Book::Result::Accepted;
}

void SVScanner::Numeric::Sign()
{

}

void SVScanner::Numeric::Details::ScaleValue()
{


    // Signed Integer:
    // -------------------------------------------------------------------------------
    // 8bits:
    if((Value >= 0) && (Value <=255))
        Size = SizeType::U8;
    else
        if((Value >= -128) && (Value <= 127))
        {
            Size = SizeType::I8;
            return;
        }
    // Signed Integer:
    // -------------------------------------------------------------------------------
    // 16bits:
    if((Value >=0) && (Value <= 65535))
        Size = SizeType::U16;
    else
        if(((Value < -128) && (Value >= -32768)) && ( Value <= 32767))
        {
            Size = SizeType::I16;
            return;
        }

    // Signed Integer:
    // -------------------------------------------------------------------------------
    // 32bits:
    if((Value >=0) && (Value <= 0xFFFFFFFF))
        Size = SizeType::U32;
    else
        if(((Value < -32768) && (Value >= -2147483648)) && ( Value <= 2147483647))
        {
            Size = SizeType::I32;
            return;
        }

    // Signed Integer:
    // -------------------------------------------------------------------------------
    // 64bits:
    if((Value < -2147483648) && (Value >= -1 * 0xFFFFFFFFFFFFFFFFL) && (Value <= std::pow(2,64))) Size = SizeType::I64;

    Size = SizeType::U64;


}
}
