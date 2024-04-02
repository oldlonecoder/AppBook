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


#include "AppBook/Book/TScanner.h"
//using namespace std::literals;
#include <AppBook/Utf/Glyphes.h>
namespace Book
{

TScanner TScanner::Numeric::Empty{};




TScanner::TScanner(std::string_view Txt)
{
    mBegin = mPos = Txt.begin()+0;
    mEnd = (mBegin + Txt.length())-1; // mEnd placed at the last valid character. It will NEVER be changed.
    mLocation = {1, 1, 0};

}


bool TScanner::PopLocation()
{
    if(mPoints.empty()) return false;
    auto P = --mPoints.end();
    mLocation.Line = P->first;
    mLocation.Col = P->second;
    mPoints.erase(P);
    return true;
}


void TScanner::PushLocation()
{
    mPoints.emplace_back(mLocation.Line, mLocation.Col);

}

bool TScanner::Eof(const char* cc)
{
    return cc > mEnd;
}


bool TScanner::Eof()
{
    return mPos > mEnd;
}


bool TScanner::SkipWS()
{
    while (!Eof() && std::isspace(*mPos)) ++mPos;
    return Eof();
}

/*!
 * @brief Synchronize internal "cursor" location {Line #, Column #, Offset} at the current iterator offset into the view.
 * @return  Reference to the updated infos into the LocationData member of Processing.
 */
TScanner::LocationData &TScanner::Sync()
{
    auto c = mBegin;
    mLocation.Line = mLocation.Col = 1; //DUH!!!!!!!
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
    //Book::Debug() << Mark();
    return mLocation;
}






TScanner::operator bool() const
{
    return mBegin != nullptr;
}


/*!
 * @brief Initiate this Processing with a (new) string_view.
 * @param view
 * @return reference to this instance.
 */
TScanner &TScanner::operator=(std::string_view view)
{

    if(view.empty())
    {
        AppBook::Error() << "Assigned with an empty string \"view\". Be aware, this instance cannot be used.";
        return *this;
    }
    mBegin = mPos = view.begin()+0;
    mEnd = mBegin + view.length()-1;
    mLocation = {1, 1, 0};
    return *this;
}



bool TScanner::operator++()
{
    if(Eof()) return false;

    ++mPos;
    //SkipWS();
    return !Eof();
}

bool TScanner::operator++(int)
{
    if(Eof()) return false;

    ++mPos;
    //SkipWS();
    return !Eof();
}

Book::Result TScanner::Seek(int32_t Idx)
{
//    if(Text.empty()) return Book::Result::Rejected;

//    Signal Exception on any attempt to seek into an empty string_view !! :
    if(mBegin && mEnd) {

        // Otherwise just reject under/overflow :
        // Otherwise execute the seek and return Accepted.
        if (Idx < 0L) {
            size_t X = -1 * Idx;
            if ((mEnd - X) < mBegin) X = 0;

            mPos = mEnd - X;
            Sync();
            return Book::Result::Accepted;
        }
        if (((mBegin + Idx) < mEnd)) mPos = mBegin + Idx;
        else
            mPos = mEnd; // Jump to the end of the text...

        Sync();
    }
    else
        AppBook::Exception() [ AppBook::Except() << Book::Result::Empty << ": Attempt to use the SeekAt method on an un-assigned/incompletely assigned TScanner!" ];

    return Eof() ? Book::Result::Eof : Book::Result::Accepted;
}


TScanner::Numeric::Result TScanner::ScanNumber()
{
    Numeric Scanner(*this);
    if (!Scanner.Base2() && !Scanner.Base8() && !Scanner.Base16() && !Scanner.Base10()) return { Book::Result::Rejected, {} };

    return { Book::Result::Accepted, Scanner.NumDetails };
}


/*!
 * @brief Scan literal string between quotes
 * @return string_view of the contents including the surrounding quotes.
 */
std::pair<Book::Result, std::string_view> TScanner::ScanLiteralString()
{
    StrAcc Buf;
    std::string_view::iterator A = mPos;


    if ((*A != '\'') && (*A != '"')) return { Book::Result::Rejected,{} };
    auto Q = *A++;

    // Ignore `""`  (empty string)
//loop_str:
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


std::string TScanner::Mark()
{
    // 1 - Locate Beginning of the line:
    StrAcc Str;
    auto LBegin = mPos;
    auto LEnd   = mPos;
    // Beginning of the line:
    while((LBegin > mBegin) && (*LBegin != '\n') && (*LBegin != '\r')) --LBegin;
    if(LBegin < mBegin) LBegin = mBegin;
    // End of line:
    while((LEnd < mEnd) && (*LEnd != '\n') && (*LEnd != '\r')) ++LEnd;
    if(LEnd > mEnd) LEnd = mEnd;
    auto col = (mPos-LBegin); // ?
    auto spc = std::string(col-1<=0? 0 : col,' ');
    //Book::Debug() << ":---> nbspace: " << spc.length() << ":";
    Str , '\n' , std::string(LBegin, LEnd+1) , '\n' , spc , Utf::Glyph::CArrowUp;
    return Str();
}




/*!
 * @brief const char* at the beginning of the Text.
 * @return string_view::iterator ( const char* );
 */
TScanner::Iterator TScanner::Begin()
{
    return mBegin; /// << The start if this Scanner
}


/*!
 * @brief const char* at the end of the Text.
 * @return string_view::iterator ( const char* );
 */
TScanner::Iterator TScanner::End()
{
    return mEnd; //< the end of this scanner.
}

void TScanner::Push()
{
    PStack.push(mPos);
}

bool TScanner::Pop()
{
    if(PStack.empty()) return false;
    mPos = PStack.top();
    PStack.pop();
    return true;
}

Book::Result TScanner::Reposition(std::size_t Offset)
{
    if(Eof()) return Book::Result::Rejected;
    mPos += Offset;
    return Result::Accepted;
}

TScanner::Iterator TScanner::StartSequence()
{
    Push();
    return mPos;
}

std::pair<TScanner::Iterator,TScanner::Iterator> TScanner::EndSequence()
{
    auto I = PStack.top();
    PStack.pop();
    return {I, mPos};
}

std::pair<TScanner::Iterator, TScanner::Iterator> TScanner::Scan(const std::function<Book::Result()>& ScannerFn)
{
    StartSequence();
    if(!!ScannerFn())
        return EndSequence();
    auto I = EndSequence();
    Back(I.first);
    return {};
}



/*!
 * @brief Scans for the first occurrence of \Seq from the current position.
 * @param Seq
 * @return Accepted if found, Rejected if not.
 * @todo Add more result codes to Book::Enum::Code !
 */
Book::Result TScanner::SeekAt(const std::string_view &Seq, int32_t Pos)
{

    Sync();
    auto sv= std::string_view(mBegin, (mEnd-mBegin)+1);
    auto Start = Pos > -1 ? Pos : mPos-mBegin; // Fuck!!!!
    auto pos = sv.find(Seq, Start);

    if(pos == std::string_view::npos)
        return Book::Result::Rejected;
    mPos = mBegin+pos;
    Sync();
    return Book::Result::Accepted;
}

Book::Result TScanner::Step(int32_t Idx)
{
    mPos += Idx;
    if(mPos > mEnd)
        return Book::Result::Eof;
    return Result::Accepted;
}

std::pair<Book::Result, std::string_view> TScanner::ScanIdentifier()
{
    auto Cursor = mPos;
    if(! std::isalpha(*Cursor) && (*Cursor != '_'))
        return {Book::Result::Rejected,{}};

    ++Cursor;
    while(!Eof() && (std::isalnum(*Cursor) || (*Cursor == '_'))) ++Cursor;
    if(Cursor > mPos)
        return {Book::Result::Accepted, {mPos, static_cast<uint32_t >(Cursor-mPos)}};

    return {Book::Result::Rejected,{}};

}


TScanner::LocationData const& TScanner::LocationData::operator>>(std::string &Out) const
{
    StrAcc OutStr="%d,%d";
    OutStr << Line << Col;//  << Offset;
    Out = OutStr();
    return *this;
}

TScanner::Numeric::Numeric(TScanner &Tx):Text(Tx), End(Tx.mEnd), Pos(Tx.mPos), Begin(Tx.mPos){}

Book::Result TScanner::Numeric::operator()()
{
    return Book::Result::Ok;
}

Book::Result TScanner::Numeric::Base2()
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
    //AppBook::Debug() << "Loop2: A on '" << Color::Yellow << *A << Color::Reset << '\'';
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
    //AppBook::Debug() << " Base2 : " << Color::Yellow << Buf << Book::Fn::Endl << " Length: " << Color::LightCoral << Seq.length();
    NumDetails.Seq = {Begin, A};
    NumDetails.Value = std::stoi(Buf(), nullptr, 2);
    NumDetails.Base = Details::BaseSize::Binary;
    NumDetails.ScaleValue();
    return Book::Result::Accepted;
}

Book::Result TScanner::Numeric::Base8()
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
    //AppBook::Debug() << "loop8: A on '" << Color::Yellow << *A << Color::Reset << '\'';
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
    //AppBook::Debug() << "Octal loop exit: A on '" << Color::Yellow << *A << Color::Reset << '\'';

    if(((std::isdigit(*A) && (*A >= '7'))) || (A==Begin))
    {
    //    AppBook::Debug() << "Octal loop exit: A on '" << Color::Yellow << *A << Color::Reset << '\'' << Color::Red4 << " Rejected";
        return Book::Result::Rejected;
    }
    //--A;
    //AppBook::Debug() << "Base8: A on '" << Color::Yellow << *A << Color::Reset << "' - Buffer: [" << Color::Yellow << Buf << Color::Reset << "]";
    NumDetails.Seq = {Begin, A};
    NumDetails.Value = std::stoi(Buf(), nullptr, 8);
    NumDetails.Base = Details::BaseSize::Octal;
    NumDetails.ScaleValue();
    return Book::Result::Accepted;

}

Book::Result TScanner::Numeric::Base10()
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
    //AppBook::Debug() << "Decimal : A on '" << Color::Yellow << *A << Color::Reset << '\'';
    if(*A == '\'') ++A;
    //AppBook::Debug() << "Decimal : A on '" << Color::Yellow << *A << Color::Reset << '\'';
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
    //AppBook::Debug() << "A on '" << Color::Yellow << *A << Color::Reset << '\'';
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

Book::Result TScanner::Numeric::Base16()
{
    //AppBook::Debug() << " Base 2,8 rejected - then:";
    auto A = Text();
    StrAcc Buf;
    NumDetails.Base = Details::BaseSize::Hexadecimal;

    if (*A == '0')
        ++A;

    if(std::toupper(*A) != 'X')
    {
    //    AppBook::Debug() << "Hex [Mandatory prefix not met]: " << Color::Red4 << "Rejected";
        return Book::Result::Rejected;
    }
    ++A;
loop16:
    //AppBook::Debug() << "loop16: A on '" << Color::Yellow << *A << Color::Reset << '\'';
    while(!Text.Eof() && std::isxdigit(*A))
    {
        Buf << *A++;

    }
    if(!Text.Eof() && (*A == '\'')){ ++A; goto loop16; }

    if(A==Begin) return Book::Result::Rejected;
    //AppBook::Debug() << "A on '" << Color::Yellow << *A << Color::Reset << '\'';
    NumDetails.Seq = {Begin, A};
//    AppBook::Debug() << "[Hex] : Sequence ["
//    << Color::Yellow << std::string(NumDetails.Seq.begin(), NumDetails.Seq.end())
//    << Color::Reset << "] Length: "
//    << Color::LightCoral << NumDetails.Seq.length()
//    << Color::White <<  " -> Buffer:["
//    << Color::Yellow << Buf
//    << Color::Reset << "] ";

    NumDetails.Value = std::stoi(Buf(), nullptr, 16);
    NumDetails.ScaleValue();
    return Book::Result::Accepted;
}

void TScanner::Numeric::Sign()
{

}

void TScanner::Numeric::Details::ScaleValue()
{


    // Signed Integer:
    // -------------------------------------------------------------------------------
    // 8bits:
    if((Value >=-128) && (Value <=127))
    {
        Size = SizeType::I8;
        return;
    }
    if((Value >= 128) && (Value <=255))
    {
            Size = SizeType::U8;
            return;
    }
    if((Value >= -32768) && (Value <= 32767))
    {
        Size = SizeType::I16;
        return;
    }
    if((Value >=32768) && (Value <= 65565))
    {
            Size = SizeType::U16;
            return;
    }
    if((Value >= -2147483648) && (Value <= 2147483647))
    {
        Size = SizeType::I32;
        return;
    }
    if((Value >= 0x10000) && (Value <= 4294967295))
    {
        Size = SizeType::U32;
        return;
    }
    if((Value < 0LL))
    {
        Size = SizeType::I64;
    }
    else
        Size = SizeType::U64;

}
}
