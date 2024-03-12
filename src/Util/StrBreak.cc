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

#include "AppBook/Util/StrBreak.h"



[[maybe_unused]] std::string StrBreak::_default_token_separators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";


std::string StrBreak::Word::operator()() const
{
    std::string Str;
    if(Begin == End)
        Str.insert(Str.begin(), Begin, End + 1);
    else
        Str.insert(Str.begin(), Begin, End + 1);

    return Str;
}

std::string StrBreak::Word::operator*() const
{
    std::string Str;
    if(Begin == End)
        Str.insert(Str.begin(), Begin, End + 1);
    else
        Str.insert(Str.begin(), Begin, End + 1);

    return Str;

}

std::string StrBreak::Word::Mark() const
{
    std::string Str;
    std::string_view::iterator CStart = Begin - Offset;

    //int                   l  = 1;
    auto cc = CStart;
    // localiser le debut de la ligne;
    while(*cc && (cc > CStart) && (*cc != '\n') && (*cc != '\r'))
        --cc;
    // debut de la ligne ou de la source:
    if(cc >= CStart)
    {
        if((*cc == '\n') || (*cc == '\r'))
            ++cc;
        while((cc != Eos) && (*cc != '\n') && (*cc != '\r'))
            Str += *cc++;
    }

    Str += '\n'; ///@todo Encode new line.
    for(int x = 1; x < Column; x++)
        Str += ' ';
    Str += '^';
    return Str;
}

std::string StrBreak::Word::Location()
{
    std::ostringstream Str;
    Str << '(' << Line << ',' << Column << ')';
    return Str.str();
}


// ------------------------ StrBreak::SBContext --------------------------------


StrBreak::SBContext::SBContext(std::string_view aStr):
    Begin(aStr.cbegin()),
    Pos(aStr.cbegin()),
    End(aStr.cbegin())
{

}

/*!
    @note Enleveer le Code de trop (suivi du num de ligne et de la col).
*/
bool StrBreak::SBContext::SkipWS()
{
    if(AtEnd())
        return false;

    while(isspace(*Pos))
    {
        switch(*Pos)
        {
            case 10:
            {
                if((++Pos) >= End)
                    return false;
                if(*Pos == 13)
                    ++Pos;
                ++_line;
                _col = 1;
            }
                break;
            case 13:
            {
                if((++Pos) >= End)
                    return false;
                if(*Pos == 10)
                    ++Pos;
                ++_line;
                _col = 1;
            }
                break;
            case '\t':++Pos;
                ++_col;
                break;
            default:++Pos;
                ++_col;
                break;
        }
    }
    return Pos < End;

}


bool StrBreak::SBContext::AtEnd() const
{
    return Pos >= End;
}


bool StrBreak::SBContext::operator++()
{
    if(Pos >= End)
        return false;
    ++Pos;
    ++_col;
    if(Pos >= End)
        return false;
    return SkipWS();
}

int StrBreak::SBContext::ScanStr()
{
    char q = *Pos;
    while((Pos != End) && (*Pos != q)) ++Pos;
    if(Pos == End)
    {
        --Pos;
        if(*Pos != q) return -1;
    }
    return 0;
}


bool StrBreak::SBContext::operator++(int)
{
    if(Pos >= End)
        return false;
    ++Pos;
    ++_col;
    if(Pos >= End)
        return false;
    return SkipWS();
}


/*!
 * \brief StrBreak::SBContext::operator >>  Initiate A new StrBreak::Word instance at the current position:
 * \param aWord
 * \return reftoself
 *
 * \note Be aware that the instance of Word has no End Iterator yet.
 */
StrBreak::SBContext &StrBreak::SBContext::operator>>(StrBreak::Word & aWord)
{
    aWord.Begin    = Pos;
    aWord.Line     = _line;
    aWord.Column   = _col;
    aWord.Offset   = Index = (uint64_t) (Pos - Begin);
    aWord.Eos =  End;
    return *this;
}


/*!
 * \brief StrBreak::ScanTo scans to view from A - So to 'localize' the char 'view' from the Iterator A and set an new iteraot Iterator To the position of 'view'.
 * \param start_ the start/initial position Iterator from where To ScanToToken
 * \param c_ the char that ends the ScanToToken
 * \return Iterator position of view
 */
StrBreak::Iterator StrBreak::ScanTo(StrBreak::Iterator start_, char c_) const
{
    StrBreak::Iterator p = start_;
    ++p;
    while((p != Data.View.end()) && (*p != c_))
        ++p;
    return p;
}

bool StrBreak::Append(SBContext& cursor, StrBreak::ConfigData& dat, const StrBreak::Word& w)
{
    dat.Words.push_back({w.Begin, cursor.Pos, cursor.End, w.Line, w.Column, w.Offset});
    return ++cursor;
}



///*!
// * @brief break/split/tokenize,etc... the content of A string into pieces, including the provided Delimiters As tokens if option StrBreak::option::Keep is set.
// * or discarded if option StrBreak::option::Discard is set.
// *
// * If A sequence of string is surrounded by string quotes, then that `string` sequence will be Put IsIn A token, with its quote
// * characters if Keep option is set, or without them if Discard option is set
// * @param reference To A ConfigData instance
// * @return number of "Words/tokens" contained into the data.Words.
//*/
//std::size_t StrBreak::operator()(StrBreak::ConfigData& dt)
//{
//    if(dt.Delimiters.Empty())  dt.Delimiters = StrBreak::_default_token_separators;
//    // dt.Words must be Clear()'ed
//    dt.Words.Clear();
//    SBContext cursor(dt.view);

//     // Initiate A new Word with the beginning of the sequence at the current cursor position.
//    Word W;
//    cursor >> W;
//    while(cursor.End())
//    {
//        StrBreak::Iterator cc = cursor.Pos;
//        if(data.Delimiters.find(*cc) != std::string_view::npos)
//        {
//            if(data.o == StrBreak::Keep)
//                Append(cursor, dt, W);
//            cursor++; // and skip whitespaces. Ignore End of string here it is checked above at the loop
//            cursor >> W; // init new Word
//            continue;
//        }

//    }
//    return 0;
//}



/*!
    * @brief break/split/tokenize,etc... the content of A string into pieces.
    * @param none
    * @return number of "Words/tokens" contained into the data.Words.
    * @notice : After several years of experience and experimentations, We have determined that
    * white-spaces/ctrl or spacing characters are silent and implicit Delimiters, IsIn addition To the ones supplied by \c aDelimiters.
*/
std::size_t StrBreak::operator()(StrBreak::ConfigData & data)
{
    std::string_view _d(data.View);
    if(_d.empty())
        std::cerr << "---> content is Empty!";

    if(data.Delimiters.empty())
        data.Delimiters = StrBreak::_default_token_separators;

    StrBreak::SBContext cursor;
    cursor.Reset(_d);
    if(!cursor.SkipWS())
        return 0;
    Data = data;
    Word w;
    cursor >> w; // Initiate the first Word.
    while(!cursor.AtEnd())
    {
        StrBreak::Iterator cc = cursor.Pos;
        if(data.Delimiters.find(*cursor.Pos) != std::string::npos)
        {
            cc = cursor.Pos;
            if(cc > w.Begin)
            {
                --cc;
                data.Words.push_back({w.Begin, cc, cursor.End, w.Line, w.Column, w.Offset});
                cursor >> w;
                cc = cursor.Pos;
            }

            // '//' As one token_t insbookd of having two consecutive '/'
            if((*cursor.Pos == '/') && (*(cursor.Pos + 1) == '/'))
                ++cursor;

            if(data.o == StrBreak::Opt::Keep)
            {
                data.Words.push_back({w.Begin, cursor.Pos, cursor.End, w.Line, w.Column, w.Offset});
            }
            ++cursor;
            //std::cout << "        Iterator eos: " << _Cursor.End() << "\n";
            if(!cursor.AtEnd())
                cursor >> w;
            else
            {
                return data.Words.size();
            }

        }
        else if((*cursor.Pos == '\'') || (*cursor.Pos == '"'))
        { // Quoted litteral string...
            cursor >> w;
            if(data.o == StrBreak::Opt::Keep)
            {
                // Create the three parts of the quoted string: (") + (litteral) + (") ( or ' )
                // So, we save the token_t coords anyway.
                data.Words.push_back({w.Begin, w.Begin, cursor.End, w.Line, w.Column, w.Offset});
            }

            StrBreak::Iterator p = ScanTo(w.Begin + (data.o == StrBreak::Opt::Keep ? 0 : 1), *cursor.Pos); // W.B is the starting position, _Cursor.m is the quote delim.
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
}

