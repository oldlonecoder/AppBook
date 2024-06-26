
#include <AppBook/Utf/Glyphes.h>
#include <map>


namespace Utf {
std::vector<AccentFR::T> AccentFR::Data =
    {
        L_agrave,
        L_acirc,
        L_cedile,
        L_eacute,
        L_egrave,
        L_ecirc,
        L_etrema,
        L_itrema,
        L_ocirc,
        L_ugrave,
        L_ucric,
        L_icirc,
        L_Agrave,
        L_Acirc,
        L_Cedile,
        L_Eacute,
        L_Egrave,
        L_Ecirc,
        L_Etrema,
        L_Itrema,
        L_Ocirc,
        L_Ugrave,
        L_Ucric,
        L_Icirc
    };
std::vector<std::string_view> AccentFR::Name =
    {
        "agrave",
        "acirc",
        "cedile",
        "eacute",
        "egrave",
        "ecirc",
        "etrema",
        "itrema",
        "ocirc",
        "ugrave",
        "ucirc",
        "icirc",
        "Agrave",
        "Acirc",
        "Cedile",
        "Eacute",
        "Egrave",
        "Ecirc",
        "Etrema",
        "Itrema",
        "Ocirc",
        "Ugrave",
        "Ucirc",
        "Icirc"
    };


AccentFR::Type AccentFR::GetEnum(std::string_view AccName)
{
    std::map<std::string_view, AccentFR::Type> Map =
        {
            {"agrave", agrave},
            {"acirc",  acirc},
            {"cedile", cedile},
            {"eacute", eacute},
            {"egrave", egrave},
            {"ecirc",  ecirc},
            {"etrema", etrema},
            {"itrema", itrema},
            {"ocirc",  ocirc},
            {"ugrave", ugrave},
            {"ucirc",  ucirc},
            {"icirc",  icirc},
            {"Agrave", Agrave},
            {"Acirc",  Acirc},
            {"Cedile", Cedile},
            {"Eacute", Eacute},
            {"Egrave", Egrave},
            {"Ecirc",  Ecirc},
            {"Etrema", Etrema},
            {"Itrema", Itrema},
            {"Ocirc",  Ocirc},
            {"Ugrave", Ugrave},
            {"Ucirc",  Ucirc},
            {"Icirc",  Icirc}
        };
    for (auto const &[k, t]: Map)
    {
        if (k == AccName) return t;
    }

    return Type::Err;
}

std::pair<bool, AccentFR::Type> AccentFR::ScanName(std::string_view AccName)
{
    std::map<std::string_view, AccentFR::Type> Map =
        {
            {"agrave", agrave},
            {"acirc",  acirc},
            {"cedile", cedile},
            {"eacute", eacute},
            {"egrave", egrave},
            {"ecirc",  ecirc},
            {"etrema", etrema},
            {"itrema", itrema},
            {"ocirc",  ocirc},
            {"ugrave", ugrave},
            {"ucirc",  ucirc},
            {"icirc",  icirc},
            {"Agrave", Agrave},
            {"Acirc",  Acirc},
            {"Cedile", Cedile},
            {"Eacute", Eacute},
            {"Egrave", Egrave},
            {"Ecirc",  Ecirc},
            {"Etrema", Etrema},
            {"Itrema", Itrema},
            {"Ocirc",  Ocirc},
            {"Ugrave", Ugrave},
            {"Ucirc",  Ucirc},
            {"Icirc",  Icirc}
        };
    for (auto const &[k, t]: Map)
    {
        if (k == AccName) return {true, t};
    }
    return {false, Type::Err};
}



Glyph::DataArray Glyph::Data =
    {
        COk                ,
        CComment           ,
        CArrowRight        ,
        CArrowLeft         ,
        CArrowUp           ,
        CArrowDown         ,
        CArrowUpRight      ,
        CArrowUpLeft       ,
        CArrowDownRight    ,
        CArrowDownLeft     ,
        CArrowHeadRight    ,
        CArrowDheadRight   ,
        CPencilDr          ,
        CPencilUr          ,
        CErr1              ,
        CFlag              ,
        CBolt              ,
        CSuccess           ,
        CSmallDot          ,
        CBigDot            ,
        CBall1             ,
        CBall2             ,
        CDeadHead          ,
        CYinYang           ,
        CSadface           ,
        CHappy1            ,
        CHappy2            ,
        CCircleX           ,
        CFunction          ,
        CCut1              ,
        CCut2              ,
        CCut3              ,
        CCMarkX            ,
        CBaseline          ,
        CStar5             ,
        CCMarkCheck        ,
        CSwordCross        ,
        CHammerCross       ,
        CSmallDot2         ,
        CErr2              ,
        CErr3              ,
        CSQuote1           ,
        CDQuoteLeft        ,
        CDuoteRight        ,
        CDowntobottomleft  ,
        CUndo              ,
        CEnter             ,
        CKeyEnter          ,
        CLinux             ,
        CArch              ,
        CHome              ,
        CFile              ,
        CThumbDown         ,
        CGithubCircle      ,
        CGithubFolder      ,
        CWindows           ,
        CPi                ,
        CFolder            ,
        CBug               ,
        CInfo              ,
        CWarning           ,
        CBomb              ,
        CEditPencilBR      ,
        CEditPencilR       ,
        CEditPencilUR      ,
        CNegCrossCheck     ,
        CShadowedCheckBox  ,
        CSpinArrowSpin0    ,
        CSpinArrowSpin1    ,
        CSpinArrowSpin2    ,
        CSpinArrowSpin3    ,
        CSpinArrowSpin4    ,
        CSpinArrowSpin5    ,
        CSpinArrowSpin6    ,
        CSpinArrowSpin7    ,
        CThumbUp           ,
        CChronos           ,
        CSand              ,
        CCoffee            ,
        CUnAuth            ,
        CHandwrite         ,
        CWorld             ,
        CWorldGrid         ,
        CSchool            ,
        CHat               ,
        CAdmin             ,
        CFabric            ,
        CCat               ,
        CFish              ,
        CSearch            ,
        CPeople            ,
        CAlien             ,
        CLight             ,
        CPoop              ,
        CCash              ,
        CComputer          ,
        CNotePad           ,
        CBooks             ,
        CSend              ,
        CRecieve           ,
        CEmail             ,
        CEmail2            ,
        CNullPtr           ,
        CStop              ,
        CDebian

    };

std::map<Glyph::Type, std::string> IcNamesMap =
    {
        {Glyph::Ok                 ,"Ok"},
        {Glyph::Comment            ,"Comment"},
        {Glyph::ArrowRight         ,"ArrowRight"},
        {Glyph::ArrowLeft          ,"ArrowLeft"},
        {Glyph::ArrowUp            ,"ArrowUp"},
        {Glyph::ArrowDown          ,"ArrowDown"},
        {Glyph::ArrowUpRight       ,"ArrowUpRight"},
        {Glyph::ArrowUpLeft        ,"ArrowUpLeft"},
        {Glyph::ArrowDownRight     ,"ArrowDownRight"},
        {Glyph::ArrowDownLeft      ,"ArrowDownLeft"},
        {Glyph::ArrowHeadRight     ,"ArrowHeadRight"},
        {Glyph::ArrowDheadRight    ,"ArrowDheadRight"},
        {Glyph::PencilDr           ,"PencilDr"},
        {Glyph::PencilUr           ,"PencilUr"},
        {Glyph::Err1               ,"Err1"},
        {Glyph::Flag               ,"Flag"},
        {Glyph::Bolt               ,"Bolt"},
        {Glyph::Success            ,"Success"},
        {Glyph::SmallDot           ,"SmallDot"},
        {Glyph::BigDot             ,"BigDot"},
        {Glyph::Ball1              ,"Ball1"},
        {Glyph::Ball2              ,"Ball2"},
        {Glyph::DeadHead           ,"DeadHead"},
        {Glyph::YinYang            ,"YinYang"},
        {Glyph::Sadface            ,"Sadface"},
        {Glyph::Happy1             ,"Happy1"},
        {Glyph::Happy2             ,"Happy2"},
        {Glyph::CircleX            ,"CircleX"},
        {Glyph::Function           ,"Function"},
        {Glyph::Cut1               ,"Cut1"},
        {Glyph::Cut2               ,"Cut2"},
        {Glyph::Cut3               ,"Cut3"},
        {Glyph::CMarkX             ,"CMarkX"},
        {Glyph::Baseline           ,"Baseline"},
        {Glyph::Star5              ,"Star5"},
        {Glyph::CMarkCheck         ,"CMarkCheck"},
        {Glyph::SwordCross         ,"SwordCross"},
        {Glyph::HammerCross        ,"HammerCross"},
        {Glyph::SmallDot2          ,"SmallDot2"},
        {Glyph::Err2               ,"Err2"},
        {Glyph::Err3               ,"Err3"},
        {Glyph::SQuote1            ,"SQuote1"},
        {Glyph::DQuoteLeft         ,"DQuoteLeft"},
        {Glyph::DQuoteRight        ,"DuoteRight"},
        {Glyph::Downtobottomleft   ,"Downtobottomleft"},
        {Glyph::Undo               ,"Undo"},
        {Glyph::Enter              ,"Enter"},
        {Glyph::KeyEnter           ,"KeyEnter"},
        {Glyph::Linux              ,"Linux"},
        {Glyph::Arch               ,"Arch"},
        {Glyph::Home               ,"Home"},
        {Glyph::File               ,"File"},
        {Glyph::ThumbDown          ,"ThumbDown"},
        {Glyph::GithubCircle       ,"GithubCircle"},
        {Glyph::GithubFolder       ,"GithubFolder"},
        {Glyph::Windows            ,"Windows"},
        {Glyph::Pi                 ,"Pi"},
        {Glyph::Folder             ,"Folder"},
        {Glyph::Bug                ,"Bug"},
        {Glyph::Info               ,"Info"},
        {Glyph::Warning            ,"Warning"},
        {Glyph::Bomb               ,"Bomb"},
        {Glyph::EditPencilBR       ,"EditPencilBR"},
        {Glyph::EditPencilR        ,"EditPencilR"},
        {Glyph::EditPencilUR       ,"EditPencilUR"},
        {Glyph::NegCrossCheck      ,"NegCrossCheck"},
        {Glyph::ShadowedCheckBox   ,"ShadowedCheckBox"},
        {Glyph::SpinArrowSpin0     ,"SpinArrowSpin0"},
        {Glyph::SpinArrowSpin1     ,"SpinArrowSpin1"},
        {Glyph::SpinArrowSpin2     ,"SpinArrowSpin2"},
        {Glyph::SpinArrowSpin3     ,"SpinArrowSpin3"},
        {Glyph::SpinArrowSpin4     ,"SpinArrowSpin4"},
        {Glyph::SpinArrowSpin5     ,"SpinArrowSpin5"},
        {Glyph::SpinArrowSpin6     ,"SpinArrowSpin6"},
        {Glyph::SpinArrowSpin7     ,"SpinArrowSpin7"},
        {Glyph::ThumbUp            ,"ThumbUp"},
        {Glyph::Chronos            ,"Chronos"},
        {Glyph::Sand               ,"Sand"},
        {Glyph::Coffee             ,"Coffee"},
        {Glyph::UnAuth             ,"UnAuth"},
        {Glyph::Handwrite          ,"Handwrite"},
        {Glyph::World              ,"World"},
        {Glyph::WorldGrid          ,"WorldGrid"},
        {Glyph::School             ,"School"},
        {Glyph::Hat                ,"Hat"},
        {Glyph::Admin              ,"Admin"},
        {Glyph::Fabric             ,"Fabric"},
        {Glyph::Cat                ,"Cat"},
        {Glyph::Fish               ,"Fish"},
        {Glyph::Search             ,"Search"},
        {Glyph::People             ,"People"},
        {Glyph::Alien              ,"Alien"},
        {Glyph::Light              ,"Light"},
        {Glyph::Poop               ,"Poop"},
        {Glyph::Cash               ,"Cash"},
        {Glyph::Computer           ,"Computer"},
        {Glyph::NotePad            ,"NotePad"},
        {Glyph::Books              ,"Books"},
        {Glyph::Send               ,"Send"},
        {Glyph::Receive            ,"Receive"},
        {Glyph::Email              ,"Email"},
        {Glyph::Email2             ,"Email2"},
        {Glyph::NullPtr            ,"NullPtr"},
        {Glyph::Stop               ,"Stop"},
        {Glyph::Debian             ,"Debian"},
    };

Glyph::Type Glyph::Scan(const std::string& GhName)
{

    for (auto& [N,Str] : IcNamesMap)
    {
        if(GhName == Str) return N;
    }
    return 0;
}

std::pair<bool, Glyph::Type> Glyph::ScanName(const std::string& GlyphName)
{
    for (auto& [N,Str] : IcNamesMap)
    {
        if(GlyphName == Str) return {true, N};
    }
    return {false, 0};
}



std::string Glyph::Name(Type Gh)
{
    return IcNamesMap[Gh];
}







}