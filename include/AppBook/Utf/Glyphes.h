//
// Created by oldlonecoder on 10/27/23.
//

#pragma once

#include <AppBook/Exports.h>

#include <vector>
#include <string>
#include <string_view>

namespace Utf
{


struct APPBOOK_EXPORTS Glyph
{

    using T = const char*;
    using Type = uint8_t;
    using DataArray = std::vector<Glyph::T>;
    static constexpr T COk                   = "✓ ";       //  Ok “✓ ”
    static constexpr T CComment              = "🗨 ";      //  Comment
    static constexpr T CArrowRight           = "➜ ";       //  ArrowRight
    static constexpr T CArrowLeft            = "⬅ ";       //  ArrowLeft
    static constexpr T CArrowUp              = "⬆ ";       //  ArrowUp
    static constexpr T CArrowDown            = "⬇ ";       //  ArrowDown
    static constexpr T CArrowUpRight         = "⬈ ";       //  ArrowUpRight
    static constexpr T CArrowUpLeft          = "⬉ ";       //  ArrowUpLeft
    static constexpr T CArrowDownRight       = "⬊ ";       //  ArrowDownRight
    static constexpr T CArrowDownLeft        = "⬋ ";       //  ArrowDownLeft
    static constexpr T CArrowHeadRight       = "➤ ";       //  ArrowHeadRight
    static constexpr T CArrowDheadRight      = "⨠ ";       //  ArrowDheadRight
    static constexpr T CPencilDr             = "✎ ";       //  PencilDr
    static constexpr T CPencilUr             = "✐ ";       //  PencilUr
    static constexpr T CErr1                 = "✘ ";       //  Err1
    static constexpr T CFlag                 = "⚑ ";       //  Flag
    static constexpr T CBolt                 = "⚡ ";       //  Butil_public
    static constexpr T CSuccess              = "👍 ";       //  Success
    static constexpr T CSmallDot             = "⚫ ";       //  SmallDot
    static constexpr T CBigDot               = "⬤ ";       //  BigDot
    static constexpr T CBall1                = "❶ ";       //  Ball1
    static constexpr T CBall2                = "❷ ";       //  Ball2
    static constexpr T CDeadHead             = "☠ ";       //  DeadHead
    static constexpr T CYinYang              = "☯ ";       //  YinYang
    static constexpr T CSadface              = "☹ ";       //  Sadface
    static constexpr T CHappy1               = "☺ ";       //  Happy1
    static constexpr T CHappy2               = "☻ ";       //  Happy2
    static constexpr T CCircleX              = "⨂ ";       //  CircleX
    static constexpr T CFunction             = "⨍ ";       //  Function
    static constexpr T CCut1                 = "✂ ";       //  Cut1
    static constexpr T CCut2                 = "✁ ";       //  Cut2
    static constexpr T CCut3                 = "✂ ";       //  Cut3
    static constexpr T CCMarkX               = "☒ ";       //  CMarkX
    static constexpr T CBaseline             = "⩡ ";       //  Baseline
    static constexpr T CStar5                = "✭ ";       //  Star5
    static constexpr T CCMarkCheck           = "☑ ";       //  CMarkCheck
    static constexpr T CSwordCross           = "⚔ ";       //  SwordCross
    static constexpr T CHammerCross          = "⚒ ";       //  HammerCross
    static constexpr T CSmallDot2            = "⚫ ";       //  SmallDot2
    static constexpr T CErr2                 = "✖ ";       //  Err2
    static constexpr T CErr3                 = "✗ ";       //  Err3
    static constexpr T CSQuote1              = "❜ ";       //  SQuote1
    static constexpr T CDQuoteLeft           = "❝ ";       //  DQuoteLeft
    static constexpr T CDuoteRight           = "❞ ";       //  DQuoteRight
    static constexpr T CDowntobottomleft     = "⟀ ";       //  Downtobottomleft
    static constexpr T CUndo                 = "⟲ ";       //  Undo
    static constexpr T CEnter                = "⤵ ";       //  Enter
    static constexpr T CKeyEnter             = "⤶ ";       //  KeyEnter
    static constexpr T CLinux                = " ";       //  Linux
    static constexpr T CArch                 = " ";       //  Arch
    static constexpr T CHome                 = " ";       //  Home
    static constexpr T CFile                 = " ";       //  File
    static constexpr T CThumbDown            = " ";       //  ThumbDown
    static constexpr T CGithubCircle         = " ";       //  GithubCircle
    static constexpr T CGithubFolder         = " ";       //  GithubFolder
    static constexpr T CWindows              = " ";       //  Windows
    static constexpr T CPi                   = " ";       //  Pi
    static constexpr T CFolder               = " ";       //  Folder
    static constexpr T CBug                  = " ";       //  Bug
    static constexpr T CInfo                 = "ⓘ ";       //  Info
    static constexpr T CWarning              = " ";       //  Warning
    static constexpr T CBomb                 = "💣 ";       //  Bomb
    static constexpr T CEditPencilBR         = "✎ ";       //  EditPencilBR
    static constexpr T CEditPencilR          = "✏ ";       //  EditPencilR
    static constexpr T CEditPencilUR         = "✐ ";       //  EditPencilUR
    static constexpr T CNegCrossCheck        = "❎ ";       //  NegCrossCheck
    static constexpr T CShadowedCheckBox     = "❏ ";       //  ShadowedCheckBox
    static constexpr T CSpinArrowSpin0       = "➩ ";       //  SpinArrowSpin0
    static constexpr T CSpinArrowSpin1       = "➪ ";       //  SpinArrowSpin1
    static constexpr T CSpinArrowSpin2       = "➫ ";       //  SpinArrowSpin2
    static constexpr T CSpinArrowSpin3       = "➬ ";       //  SpinArrowSpin3
    static constexpr T CSpinArrowSpin4       = "➭ ";       //  SpinArrowSpin4
    static constexpr T CSpinArrowSpin5       = "➮ ";       //  SpinArrowSpin5
    static constexpr T CSpinArrowSpin6       = "➯ ";       //  SpinArrowSpin6
    static constexpr T CSpinArrowSpin7       = "➱ ";       //  SpinArrowSpin7
    static constexpr T CThumbUp              = "👍 ";       //  ThumbUp
    static constexpr T CChronos              = "⏱ ";       //  Chronos
    static constexpr T CSand                 = "⏳ ";       //  Sand
    static constexpr T CCoffee               = "☕ ";       //  Coffee
    static constexpr T CUnAuth               = "⛔ ";       //  UnAuth
    static constexpr T CHandwrite            = "✍ ";       //  Handwrite
    static constexpr T CWorld                = "🌎 ";      //  World
    static constexpr T CWorldGrid            = "🌐 ";      //  WorldGrid
    static constexpr T CSchool               = "🎓 ";      //  School
    static constexpr T CHat                  = "🎩 ";      //  Hat
    static constexpr T CAdmin                = "🏛 ";      //  Admin
    static constexpr T CFabric               = "🏭 ";      //  Fabric
    static constexpr T CCat                  = "🐈 ";      //  Cat
    static constexpr T CFish                 = "🐟 ";      //  Fish
    static constexpr T CSearch               = "👓 ";      //  Search
    static constexpr T CPeople               = "👪 ";      //  People
    static constexpr T CAlien                = "👽 ";      //  Alien
    static constexpr T CLight                = "💡 ";      //  Light
    static constexpr T CPoop                 = "💩 ";      //  Poop
    static constexpr T CCash                 = "💰 ";      //  Cash
    static constexpr T CComputer             = "💻 ";      //  Computer
    static constexpr T CNotePad              = "📋 ";      //  NotePad
    static constexpr T CBooks                = "📚 ";      //  Books
    static constexpr T CSend                 = "📤 ";      //  Send
    static constexpr T CRecieve              = "📥 ";      //  Recieve
    static constexpr T CEmail                = "📫 ";      //  Email
    static constexpr T CEmail2               = "📬 ";      //  Email2
    static constexpr T CNullPtr              = "👽 ";      //  NullPtr
    static constexpr T CStop                 = "⏹ ";      //Stop;

    static constexpr Type Ok                 = 0;
    static constexpr Type Comment            = 1;
    static constexpr Type ArrowRight         = 2;
    static constexpr Type ArrowLeft          = 3;
    static constexpr Type ArrowUp            = 4;
    static constexpr Type ArrowDown          = 5;
    static constexpr Type ArrowUpRight       = 6;
    static constexpr Type ArrowUpLeft        = 7;
    static constexpr Type ArrowDownRight     = 8;
    static constexpr Type ArrowDownLeft      = 9;
    static constexpr Type ArrowHeadRight     = 10;
    static constexpr Type ArrowDheadRight    = 11;
    static constexpr Type PencilDr           = 12;
    static constexpr Type PencilUr           = 13;
    static constexpr Type Err1               = 14;
    static constexpr Type Flag               = 15;
    static constexpr Type Bolt               = 16;
    static constexpr Type Success            = 17;
    static constexpr Type SmallDot           = 18;
    static constexpr Type BigDot             = 19;
    static constexpr Type Ball1              = 20;
    static constexpr Type Ball2              = 21;
    static constexpr Type DeadHead           = 22;
    static constexpr Type YinYang            = 23;
    static constexpr Type Sadface            = 24;
    static constexpr Type Happy1             = 25;
    static constexpr Type Happy2             = 26;
    static constexpr Type CircleX            = 27;
    static constexpr Type Function           = 28;
    static constexpr Type Cut1               = 29;
    static constexpr Type Cut2               = 30;
    static constexpr Type Cut3               = 31;
    static constexpr Type CMarkX             = 32;
    static constexpr Type Baseline           = 33;
    static constexpr Type Star5              = 34;
    static constexpr Type CMarkCheck         = 35;
    static constexpr Type SwordCross         = 36;
    static constexpr Type HammerCross        = 37;
    static constexpr Type SmallDot2          = 38;
    static constexpr Type Err2               = 39;
    static constexpr Type Err3               = 40;
    static constexpr Type SQuote1            = 41;
    static constexpr Type DQuoteLeft         = 42;
    static constexpr Type DQuoteRight        = 43;
    static constexpr Type Downtobottomleft   = 44;
    static constexpr Type Undo               = 45;
    static constexpr Type Enter              = 46;
    static constexpr Type KeyEnter           = 47;
    static constexpr Type Linux              = 48;
    static constexpr Type Arch               = 49;
    static constexpr Type Home               = 50;
    static constexpr Type File               = 51;
    static constexpr Type ThumbDown          = 52;
    static constexpr Type GithubCircle       = 53;
    static constexpr Type GithubFolder       = 54;
    static constexpr Type Windows            = 55;
    static constexpr Type Pi                 = 56;
    static constexpr Type Folder             = 57;
    static constexpr Type Bug                = 58;
    static constexpr Type Info               = 59;
    static constexpr Type Warning            = 60;
    static constexpr Type Bomb               = 61;
    static constexpr Type EditPencilBR       = 62;
    static constexpr Type EditPencilR        = 63;
    static constexpr Type EditPencilUR       = 64;
    static constexpr Type NegCrossCheck      = 65;
    static constexpr Type ShadowedCheckBox   = 66;
    static constexpr Type SpinArrowSpin0     = 67;
    static constexpr Type SpinArrowSpin1     = 68;
    static constexpr Type SpinArrowSpin2     = 69;
    static constexpr Type SpinArrowSpin3     = 70;
    static constexpr Type SpinArrowSpin4     = 71;
    static constexpr Type SpinArrowSpin5     = 72;
    static constexpr Type SpinArrowSpin6     = 73;
    static constexpr Type SpinArrowSpin7     = 74;
    static constexpr Type ThumbUp            = 75;
    static constexpr Type Chronos            = 76;
    static constexpr Type Sand               = 77;
    static constexpr Type Coffee             = 78;
    static constexpr Type UnAuth             = 79;
    static constexpr Type Handwrite          = 80;
    static constexpr Type World              = 81;
    static constexpr Type WorldGrid          = 82;
    static constexpr Type School             = 83;
    static constexpr Type Hat                = 84;
    static constexpr Type Admin              = 85;
    static constexpr Type Fabric             = 86;
    static constexpr Type Cat                = 87;
    static constexpr Type Fish               = 88;
    static constexpr Type Search             = 89;
    static constexpr Type People             = 90;
    static constexpr Type Alien              = 91;
    static constexpr Type Light              = 92;
    static constexpr Type Poop               = 93;
    static constexpr Type Cash               = 94;
    static constexpr Type Computer           = 95;
    static constexpr Type NotePad            = 96;
    static constexpr Type Books              = 97;
    static constexpr Type Send               = 98;
    static constexpr Type Recieve            = 99;
    static constexpr Type Email              = 100;
    static constexpr Type Email2             = 101;
    static constexpr Type NullPtr            = 102;
    static constexpr Type Stop               = 103;
    static Glyph::DataArray Data;

    static Glyph::Type Scan(std::string GlyphName);
    static std::string Name(Glyph::Type Gh);
};








/*

    https://www.instructables.com/Programming--how-to-detect-and-read-UTF-8-charact/
// utf8 points to A byte of A STMLText string
// Uni  points to A variable which will store the Unicode
// the function returns how many byte have been read

int UTF8_to_Unicode ( char * utf8, unsigned int * Uni )
{
    if ( utf8 == NULL ) return 0;
    if ( Uni  == NULL ) return 0;
    // U-00000000 - U-0000007F
    // ASCII Code ?
    if (*utf8 >= 0)
    {
        *Uni= *utf8; return 1;
    }

    int Len=0;
    unsigned char * u = (unsigned char *)utf8;
    *Uni = 0;

    // U-00000080 - U-000007FF : 110xxxxx 10xxxxxx
    if ( (u[0]&0xE0) == 0xC0 )
    {
        Len = 2; *Uni = u[0]&0x1F;
    }
    else
    // U-00000800 - U-0000FFFF : 1110xxxx 10xxxxxx 10xxxxxx
        if ( (u[0]&0xF0) == 0xE0 )
        {
            Len = 3;
            *Uni = u[0]&0x0F;
        }
        else
        // U-00010000 - U-001FFFFF : 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            if ( (u[0]&0xF8) == 0xF0 )
            {
                Len = 4;
                *Uni = u[0]&0x07;
            }
            else
            {
            // our UTF-8 character is malformed
            // let&aposs return it as an extended-ASCII
                *Uni = u[0];
                return 1;
            }
            // we&aposre going to read the following bytes
        int A;
        for ( A=1; A<Len; A++ )
        {
            if ( ( u[A] >=0 ) || ( (u[A]&0xC0) != 0x80 ) )
            {
            // our UTF-8 Code is malformed ...
            // let&aposs return it as an extended-ASCII
            *Uni = u[0];
            return 1;
        }
        // so far, everything seems Ok.
        // we safely build our Unicode
        *Uni = (*Uni<<6) | (u[A]&0x3F);
    }
    // According to Unicode 5.0
    // codes in the range 0xD800 to 0xDFFF
    // are not allowed.
    if ( ( (*Uni) >= 0xD800 ) || ( (*Uni) <= 0xDFFF ) )
    {
        // In this case, our UTF-8 Code was well formed.
        // So, or we break it into 2 extended ASCII codes,
        // or we display an other symbol insbookd ...
        // We should read the Unicode 5.0 book to
        // to know their official recommendations though ...
        *Uni = &apos?&apos;
        return 1;
    }
    // it&aposs done !
    // *Uni contains our unicode.
    // we simply return how many bytes
    // it was stored in.
    return Len;
}

Good explanation!

I know this is old, but there are A couple of bugs in your sample Code.

 *u[A] >= 0 always evaluates to true if u is unsigned. You need A cast to char.
if ((char)*u[A] > 0) ... )

 Checking the range U+D800 0 U+DFFF should use && not ||
if ((*Uni >= 0xD800) && (*Uni <= 0xDFFF))

 There'Code A superfluous semicolon at the End of the while loop'Code closing brace in UTF8_strlen
Also, Uni should be defined As unsigned int IsIn the same function (To match the other function'Code argument.

It would be nice if you could reformat the Code so that line breaks appear correctly. I suppose it got mangled by the editor...

For anyone interested, here'Code the fixed version (I hope you don't mind, I'll remove it if any objections).



*/












struct APPBOOK_EXPORTS AccentFR
{
    using T = const char*;

    enum Type : uint8_t
    {
        agrave ,
        acirc  ,
        cedile ,
        eacute ,
        egrave ,
        ecirc  ,
        etrema ,
        itrema ,
        ocirc  ,
        ugrave ,
        ucirc  ,
        icirc   ,
        Agrave  ,
        Acirc   ,
        Cedile  ,
        Eacute  ,
        Egrave  ,
        Ecirc   ,
        Etrema  ,
        Itrema  ,
        Ocirc   ,
        Ugrave  ,
        Ucirc   ,
        Icirc   ,
        Err

    };

    static constexpr T L_agrave = "à";
    static constexpr T L_acirc  = "â";
    static constexpr T L_cedile = "ç";
    static constexpr T L_eacute = "é";
    static constexpr T L_egrave = "è";
    static constexpr T L_ecirc  = "ê";
    static constexpr T L_etrema = "ë";
    static constexpr T L_itrema = "ï";
    static constexpr T L_ocirc  = "ô";
    static constexpr T L_ugrave = "ù";
    static constexpr T L_ucric  = "û";
    static constexpr T L_icirc  = "î";

    static constexpr T L_Agrave = "À";
    static constexpr T L_Acirc  = "Â";
    static constexpr T L_Cedile = "Ç";
    static constexpr T L_Eacute = "É";
    static constexpr T L_Egrave = "È";
    static constexpr T L_Ecirc  = "Ê";
    static constexpr T L_Etrema = "Ë";
    static constexpr T L_Itrema = "Ï";
    static constexpr T L_Ocirc  = "Ô";
    static constexpr T L_Ugrave = "Ù";
    static constexpr T L_Ucric  = "Û";
    static constexpr T L_Icirc  = "Î";

    static std::vector<AccentFR::T> Data;
    static std::vector<std::string_view> Name;
    static AccentFR::Type GetEnum(std::string_view AccName);
};





// ------------------------------------  Glyphes -----------------------------------------------------
















}// -- namespace Utf;