//
// Created by oldlonecoder on 10/27/23.
//
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <cmath>
#include <functional>
#include <string>
#include <string_view>
#include <stack>
#include <unordered_map>
#include <map>

#include "AppBook/Util/Colors.h"

//namespace Util {

std::vector<Color::Data> ColorDB =
{
    {Color::Black,            "#000"/*#000000	*/, 0x00, 0x00, 0x00},//#000000	rgb(128,0,0)	hsl(0,100%,25%)
    {Color::Maroon,           "#800000"/*#800000	*/, 0x80, 0x00, 0x00},//#800000	rgb(128,0,0)	hsl(0,100%,25%)
    {Color::Green,            "Green"/*#008000	*/, 0x00, 0x80, 0x00},
    {Color::Olive,            "Olive"/*#808000	*/, 0x80, 0x80, 0x00},//#808000	rgb(128,128,0)	hsl(60,100%,25%)
    {Color::Navy,             "Navy"/*#000080	*/, 0x00, 0x00, 0x80},//#000080	rgb(0,0,128)	hsl(240,100%,25%)
    {Color::Purple,           "Purple"/*#800080	*/, 0x80, 0x00, 0x80},//#800080	rgb(128,0,128)	hsl(300,100%,25%)
    {Color::Cyan,             "Cyan"/*#008080	*/, 0x00, 0x80, 0x80},//#008080	rgb(0,128,128)	hsl(180,100%,25%)
    {Color::Silver,           "Silver"/*#c0c0c0	*/, 0xc0, 0xc0, 0xc0},//#c0c0c0	rgb(192,192,192)	hsl(0,0%,75%)
    {Color::Grey,             "Grey"/*#808080	*/, 0x80, 0x80, 0x80},//#808080	rgb(128,128,128)	hsl(0,0%,50%)
    {Color::Red,              "Red"/*#ff0000	rgb(*/, 0xff, 0x00, 0x00},//#ff0000	rgb(255,0,0)	hsl(0,100%,50%)
    {Color::Lime,             "Lime"/*#00ff00	*/, 0x00, 0xff, 0x00},//#00ff00	rgb(0,255,0)	hsl(120,100%,50%)
    {Color::Yellow,           "Yellow"/*#ffff00	*/, 0xff, 0xff, 0x00},//#ffff00	rgb(255,255,0)	hsl(60,100%,50%)
    {Color::Blue,             "Blue"/*#0000ff	*/, 0x00, 0x00, 0xff},//#0000ff	rgb(0,0,255)	hsl(240,100%,50%)
    {Color::Fuchsia,          "Fuchsia"/*#ff00ff	*/, 0xff, 0x00, 0xff},//#ff00ff	rgb(255,0,255)	hsl(300,100%,50%)
    {Color::Aqua,             "Aqua"/*#00ffff	*/, 0x00, 0xff, 0xff},//#00ffff	rgb(0,255,255)	hsl(180,100%,50%)
    {Color::White,            "White"/*#ffffff	*/, 0xff, 0xff, 0xff},//#ffffff	rgb(255,255,255)	hsl(0,0%,100%)
    {Color::Grey0,            "Grey0"/*#000000	*/, 0x00, 0x00, 0x00},//#000000	rgb(0,0,0)	hsl(0,0%,0%)
    {Color::NavyBlue,         "NavyBlue"/*#00005f*/, 0x00, 0x00, 0x5f},//#00005f	rgb(0,0,95)	hsl(240,100%,18%)
    {Color::DarkBlue,         "DarkBlue"/*#000087*/, 0x00, 0x00, 0x87},//#000087	rgb(0,0,135)	hsl(240,100%,26%)
    {Color::Blue3,            "Blue3"/*#0000af	*/, 0x00, 0x00, 0xaf},//#0000af	rgb(0,0,175)	hsl(240,100%,34%)
    {Color::Blue4,            "Blue4"/*#0000d7	*/, 0x00, 0x00, 0xd7},//#0000d7	rgb(0,0,215)	hsl(240,100%,42%)
    {Color::Blue1,            "Blue1"/*#0000ff	*/, 0x00, 0x00, 0xff},//#0000ff	rgb(0,0,255)	hsl(240,100%,50%)
    {Color::DarkGreen,        "DarkGreen"/*#005f0*/, 0x00, 0x5f, 0x00},//#005f00	rgb(0,95,0)	hsl(120,100%,18%)
    {Color::DeepSkyBlue4,     "DeepSkyBlue4"/*#00*/, 0x00, 0x5f, 0x5f},//#005f5f	rgb(0,95,95)	hsl(180,100%,18%)
    {Color::DeepSkyBlue5,     "DeepSkyBlue5"/*#00*/, 0x00, 0x5f, 0x87},//#005f87	rgb(0,95,135)	hsl(97,100%,26%)
    {Color::DeepSkyBlue6,     "DeepSkyBlue6"/*#00*/, 0x00, 0x5f, 0xaf},//#005faf	rgb(0,95,175)	hsl(07,100%,34%)
    {Color::DodgerBlue3,      "DodgerBlue3"/*#005*/, 0x00, 0x5f, 0xd7},//#005fd7	rgb(0,95,215)	hsl(13,100%,42%)
    {Color::DodgerBlue2,      "DodgerBlue2"/*#005*/, 0x00, 0x5f, 0xff},//#005fff	rgb(0,95,255)	hsl(17,100%,50%)
    {Color::Green4,           "Green4"/*#008700	*/, 0x00, 0x87, 0x00},//#008700	rgb(0,135,0)	hsl(120,100%,26%)
    {Color::SpringGreen4,     "SpringGreen4"/*#00*/, 0x00, 0x87, 0x5f},//#00875f	rgb(0,135,95)	hsl(62,100%,26%)
    {Color::Turquoise4,       "Turquoise4"/*#0087*/, 0x00, 0x87, 0x87},//#008787	rgb(0,135,135)	hsl(180,100%,26%)
    {Color::DeepSkyBlue3,     "DeepSkyBlue3"/*#00*/, 0x00, 0x87, 0xaf},//#0087af	rgb(0,135,175)	hsl(93,100%,34%)
    {Color::DeepSkyBlue7,     "DeepSkyBlue7"/*#00*/, 0x00, 0x87, 0xd7},//#0087d7	rgb(0,135,215)	hsl(02,100%,42%)
    {Color::DodgerBlue1,      "DodgerBlue1"/*#008*/, 0x00, 0x87, 0xff},//#0087ff	rgb(0,135,255)	hsl(08,100%,50%)
    {Color::Green3,           "Green3"/*#00af00	*/, 0x00, 0xaf, 0x00},//#00af00	rgb(0,175,0)	hsl(120,100%,34%)
    {Color::SpringGreen3,     "SpringGreen3"/*#00*/, 0x00, 0xaf, 0x5f},//#00af5f	rgb(0,175,95)	hsl(52,100%,34%)
    {Color::DarkCyan,         "DarkCyan"/*#00af87*/, 0x00, 0xaf, 0x87},//#00af87	rgb(0,175,135)	hsl(66,100%,34%)
    {Color::LightSeaGreen,    "LightSeaGreen"/*#0*/, 0x00, 0xaf, 0xaf},//#00afaf	rgb(0,175,175)	hsl(180,100%,34%)
    {Color::DeepSkyBlue2,     "DeepSkyBlue2"/*#00*/, 0x00, 0xaf, 0xd7},//#00afd7	rgb(0,175,215)	hsl(91,100%,42%)
    {Color::DeepSkyBlue1,     "DeepSkyBlue1"/*#00*/, 0x00, 0xaf, 0xff},//#00afff	rgb(0,175,255)	hsl(98,100%,50%)
    {Color::Green5,           "Green5"/*#00d700	*/, 0x00, 0xd7, 0x00},//#00d700	rgb(0,215,0)	hsl(120,100%,42%)
    {Color::SpringGreen5,     "SpringGreen5"/*#00*/, 0x00, 0xd7, 0x5f},//#00d75f	rgb(0,215,95)	hsl(46,100%,42%)
    {Color::SpringGreen2,     "SpringGreen2"/*#00*/, 0x00, 0xd7, 0x87},//#00d787	rgb(0,215,135)	hsl(57,100%,42%)
    {Color::Cyan3,            "Cyan3"/*#00d7af	*/, 0x00, 0xd7, 0xaf},//#00d7af	rgb(0,215,175)	hsl(68,100%,42%)
    {Color::DarkTurquoise,    "DarkTurquoise"/*#0*/, 0x00, 0xd7, 0xd7},//#00d7d7	rgb(0,215,215)	hsl(180,100%,42%)
    {Color::Turquoise2,       "Turquoise2"/*#00d7*/, 0x00, 0xd7, 0xff},//#00d7ff	rgb(0,215,255)	hsl(89,100%,50%)
    {Color::Green1,           "Green1"/*#00ff00	*/, 0x00, 0xff, 0x00},//#00ff00	rgb(0,255,0)	hsl(120,100%,50%)
    {Color::SpringGreen6,     "SpringGreen6"/*#00*/, 0x00, 0xff, 0x5f},//#00ff5f	rgb(0,255,95)	hsl(42,100%,50%)
    {Color::SpringGreen1,     "SpringGreen1"/*#00*/, 0x00, 0xff, 0x87},//#00ff87	rgb(0,255,135)	hsl(51,100%,50%)
    {Color::MediumSpringGreen,"MediumSpringGreen"/* */, 0x00, 0xff, 0xff},//#00ffaf	rgb(0,255,175)	hsl(61,100%,50%)
    {Color::Cyan2,            "Cyan2"/*#00ffd7	*/, 0x00, 0xff, 0xd7},//#00ffd7	rgb(0,255,215)	hsl(70,100%,50%)
    {Color::Cyan1,            "Cyan1"/*#00ffff	*/, 0x00, 0xff, 0xff},//#00ffff	rgb(0,255,255)	hsl(180,100%,50%)
    {Color::DarkRed,          "DarkRed"/*#5f0000	*/, 0x5f, 0x00, 0x00},//#5f0000	rgb(95,0,0)	hsl(0,100%,18%)
    {Color::DeepPink4,        "DeepPink4"/*#5f005*/, 0x5f, 0x00, 0x5f},//#5f005f	rgb(95,0,95)	hsl(300,100%,18%)
    {Color::Purple4,          "Purple4"/*#5f0087	*/, 0x5f, 0x00, 0x87},//#5f0087	rgb(95,0,135)	hsl(82,100%,26%)
    {Color::Purple5,          "Purple5"/*#5f00af	*/, 0x5f, 0x00, 0xaf},//#5f00af	rgb(95,0,175)	hsl(72,100%,34%)
    {Color::Purple3,          "Purple3"/*#5f00d7	*/, 0x5f, 0x00, 0xd7},//#5f00d7	rgb(95,0,215)	hsl(66,100%,42%)
    {Color::BlueViolet,       "BlueViolet"/*#5f00*/, 0x5f, 0x00, 0xff},//#5f00ff	rgb(95,0,255)	hsl(62,100%,50%)
    {Color::Orange4,          "Orange4"/*#5f5f00	*/, 0x5f, 0x5f, 0x00},//#5f5f00	rgb(95,95,0)	hsl(60,100%,18%)
    {Color::Grey37,           "Grey37"/*#5f5f5f	*/, 0x5f, 0x5f, 0x5f},//#5f5f5f	rgb(95,95,95)	hsl(0,0%,37%)
    {Color::MediumPurple4,    "MediumPurple4"/*#5*/, 0x5f, 0x5f, 0x87},//#5f5f87	rgb(95,95,135)	hsl(240,17%,45%)
    {Color::SlateBlue3,       "SlateBlue3"/*#5f5f*/, 0x5f, 0x5f, 0xaf},//#5f5faf	rgb(95,95,175)	hsl(240,33%,52%)
    {Color::SlateBlue4,       "SlateBlue4"/*#5f5f*/, 0x5f, 0x5f, 0xd7},//#5f5fd7	rgb(95,95,215)	hsl(240,60%,60%)
    {Color::RoyalBlue1,       "RoyalBlue1"/*#5f5f*/, 0x5f, 0x5f, 0xff},//#5f5fff	rgb(95,95,255)	hsl(240,100%,68%)
    {Color::Chartreuse4,      "Chartreuse4"/*#5f8*/, 0x5f, 0x87, 0x00},//#5f8700	rgb(95,135,0)	hsl(7,100%,26%)
    {Color::DarkSeaGreen4,    "DarkSeaGreen4"/*#5*/, 0x5f, 0x87, 0x5f},//#5f875f	rgb(95,135,95)	hsl(120,17%,45%)
    {Color::PaleTurquoise4,   "PaleTurquoise4"/*#*/, 0x5f, 0x87, 0x87},//#5f8787	rgb(95,135,135)	hsl(180,17%,45%)
    {Color::SteelBlue,        "SteelBlue"/*#5f87a*/, 0x5f, 0x87, 0xaf},//#5f87af	rgb(95,135,175)	hsl(210,33%,52%)
    {Color::SteelBlue3,       "SteelBlue3"/*#5f87*/, 0x5f, 0x87, 0xd7},//#5f87d7	rgb(95,135,215)	hsl(220,60%,60%)
    {Color::CornflowerBlue,   "CornflowerBlue"/*#*/, 0x5f, 0x87, 0xff},//#5f87ff	rgb(95,135,255)	hsl(225,100%,68%)
    {Color::Chartreuse3,      "Chartreuse3"/*#5fa*/, 0x5f, 0xaf, 0x00},//#5faf00	rgb(95,175,0)	hsl(7,100%,34%)
    {Color::DarkSeaGreen5,    "DarkSeaGreen5"/*#5*/, 0x5f, 0xaf, 0x5f},//#5faf5f	rgb(95,175,95)	hsl(120,33%,52%)
    {Color::CadetBlue,        "CadetBlue"/*#5faf8*/, 0x5f, 0xaf, 0x87},//#5faf87	rgb(95,175,135)	hsl(150,33%,52%)
    {Color::CadetBlue2,       "CadetBlue2"/*#5faf*/, 0x5f, 0xaf, 0xaf},//#5fafaf	rgb(95,175,175)	hsl(180,33%,52%)
    {Color::SkyBlue3,         "SkyBlue3"/*#5fafd7*/, 0x5f, 0xaf, 0xd7},//#5fafd7	rgb(95,175,215)	hsl(200,60%,60%)
    {Color::SteelBlue1,       "SteelBlue1"/*#5faf*/, 0x5f, 0xaf, 0xff},//#5fafff	rgb(95,175,255)	hsl(210,100%,68%)
    {Color::Chartreuse5,      "Chartreuse5"/*#5fd*/, 0x5f, 0xd7, 0x00},//#5fd700	rgb(95,215,0)	hsl(3,100%,42%)
    {Color::PaleGreen3,       "PaleGreen3"/*#5fd7*/, 0x5f, 0xd7, 0x5f},//#5fd75f	rgb(95,215,95)	hsl(120,60%,60%)
    {Color::SeaGreen3,        "SeaGreen3"/*#5fd78*/, 0x5f, 0xd7, 0x87},//#5fd787	rgb(95,215,135)	hsl(140,60%,60%)
    {Color::Aquamarine3,      "Aquamarine3"/*#5fd*/, 0x5f, 0xd7, 0xaf},//#5fd7af	rgb(95,215,175)	hsl(160,60%,60%)
    {Color::MediumTurquoise,  "MediumTurquoise"/**/, 0x5f, 0xd7, 0xd7},//#5fd7d7	rgb(95,215,215)	hsl(180,60%,60%)
    {Color::SteelBlue4,       "SteelBlue4"/*#5fd7*/, 0x5f, 0xd7, 0xff},//#5fd7ff	rgb(95,215,255)	hsl(195,100%,68%)
    {Color::Chartreuse2,      "Chartreuse2"/*#5ff*/, 0x5f, 0xff, 0x00},//#5fff00	rgb(95,255,0)	hsl(7,100%,50%)
    {Color::SeaGreen2,        "SeaGreen2"/*#5fff5*/, 0x5f, 0xff, 0x5f},//#5fff5f	rgb(95,255,95)	hsl(120,100%,68%)
    {Color::SeaGreen1,        "SeaGreen1"/*#5fff8*/, 0x5f, 0xff, 0x87},//#5fff87	rgb(95,255,135)	hsl(135,100%,68%)
    {Color::SeaGreen4,        "SeaGreen4"/*#5fffa*/, 0x5f, 0xff, 0xaf},//#5fffaf	rgb(95,255,175)	hsl(150,100%,68%)
    {Color::Aquamarine1,      "Aquamarine1"/*#5ff*/, 0x5f, 0xff, 0xd7},//#5fffd7	rgb(95,255,215)	hsl(165,100%,68%)
    {Color::DarkSlateGray2,   "DarkSlateGray2"/*#*/, 0x5f, 0xff, 0xff},//#5fffff	rgb(95,255,255)	hsl(180,100%,68%)
    {Color::DarkRed2,         "DarkRed2"/*#870000*/, 0x87, 0x00, 0x00},//#870000	rgb(135,0,0)	hsl(0,100%,26%)
    {Color::DeepPink5,        "DeepPink5"/*#87005*/, 0x87, 0x00, 0x5f},//#87005f	rgb(135,0,95)	hsl(17,100%,26%)
    {Color::DarkMagenta,      "DarkMagenta"/*#870*/, 0x87, 0x00, 0x87},//#870087	rgb(135,0,135)	hsl(300,100%,26%)
    {Color::DarkMagenta2,     "DarkMagenta2"/*#87*/, 0x87, 0x00, 0xaf},//#8700af	rgb(135,0,175)	hsl(86,100%,34%)
    {Color::DarkViolet,       "DarkViolet"/*#8700*/, 0x87, 0x00, 0xd7},//#8700d7	rgb(135,0,215)	hsl(77,100%,42%)
    {Color::Purple2,          "Purple2"/*#8700ff	*/, 0x87, 0x00, 0xff},//#8700ff	rgb(135,0,255)	hsl(71,100%,50%)
    {Color::Orange5,          "Orange5"/*#875f00	*/, 0x87, 0x5f, 0x00},//#875f00	rgb(135,95,0)	hsl(2,100%,26%)
    {Color::LightPink4,       "LightPink4"/*#875f*/, 0x87, 0x5f, 0x5f},//#875f5f	rgb(135,95,95)	hsl(0,17%,45%)
    {Color::Plum4,            "Plum4"/*#875f87	*/, 0x87, 0x5f, 0x87},//#875f87	rgb(135,95,135)	hsl(300,17%,45%)
    {Color::MediumPurple3,    "MediumPurple3"/*#8*/, 0x87, 0x5f, 0xaf},//#875faf	rgb(135,95,175)	hsl(270,33%,52%)
    {Color::MediumPurple5,    "MediumPurple5"/*#8*/, 0x87, 0x5f, 0xd7},//#875fd7	rgb(135,95,215)	hsl(260,60%,60%)
    {Color::SlateBlue1,       "SlateBlue1"/*#875f*/, 0x87, 0x5f, 0xff},//#875fff	rgb(135,95,255)	hsl(255,100%,68%)
    {Color::Yellow4,          "Yellow4"/*#878700	*/, 0x87, 0x87, 0x00},//#878700	rgb(135,135,0)	hsl(60,100%,26%)
    {Color::Wheat4,           "Wheat4"/*#87875f	*/, 0x87, 0x87, 0x5f},//#87875f	rgb(135,135,95)	hsl(60,17%,45%)
    {Color::Grey53,           "Grey53"/*#878787	*/, 0x87, 0x87, 0x87},//#878787	rgb(135,135,135)	hsl(0,0%,52%)
    {Color::LighcoreateGrey,   "LighcoreateGrey"/*#*/, 0x87, 0x87, 0xaf},//#8787af	rgb(135,135,175)	hsl(240,20%,60%)
    {Color::MediumPurple,     "MediumPurple"/*#87*/, 0x87, 0x87, 0xd7},//#8787d7	rgb(135,135,215)	hsl(240,50%,68%)
    {Color::LighcoreateBlue,   "LighcoreateBlue"/*#*/, 0x87, 0x87, 0xff},//#8787ff	rgb(135,135,255)	hsl(240,100%,76%)
    {Color::Yellow5,          "Yellow5"/*#87af00	*/, 0x87, 0xaf, 0x00},//#87af00	rgb(135,175,0)	hsl(3,100%,34%)
    {Color::DarkOliveGreen3,  "DarkOliveGreen3"/**/, 0x87, 0xaf, 0x5f},//#87af5f	rgb(135,175,95)	hsl(90,33%,52%)
    {Color::DarkSeaGreen,     "DarkSeaGreen"/*#87*/, 0x87, 0xaf, 0x87},//#87af87	rgb(135,175,135)	hsl(120,20%,60%)
    {Color::LightSkyBlue3,    "LightSkyBlue3"/*#8*/, 0x87, 0xaf, 0xaf},//#87afaf	rgb(135,175,175)	hsl(180,20%,60%)
    {Color::LightSkyBlue4,    "LightSkyBlue4"/*#8*/, 0x87, 0xaf, 0xd7},//#87afd7	rgb(135,175,215)	hsl(210,50%,68%)
    {Color::SkyBlue2,         "SkyBlue2"/*#87afff*/, 0x87, 0xaf, 0xff},//#87afff	rgb(135,175,255)	hsl(220,100%,76%)
    {Color::Chartreuse6,      "Chartreuse6"/*#87d*/, 0x87, 0xd7, 0x00},//#87d700	rgb(135,215,0)	hsl(2,100%,42%)
    {Color::DarkOliveGreen4,  "DarkOliveGreen4"/**/, 0x87, 0xd7, 0x5f},//#87d75f	rgb(135,215,95)	hsl(100,60%,60%)
    {Color::PaleGreen4,       "PaleGreen4"/*#87d7*/, 0x87, 0xd7, 0x87},//#87d787	rgb(135,215,135)	hsl(120,50%,68%)
    {Color::DarkSeaGreen3,    "DarkSeaGreen3"/*#8*/, 0x87, 0xd7, 0xaf},//#87d7af	rgb(135,215,175)	hsl(150,50%,68%)
    {Color::DarkSlateGray3,   "DarkSlateGray3"/*#*/, 0x87, 0xd7, 0xd7},//#87d7d7	rgb(135,215,215)	hsl(180,50%,68%)
    {Color::SkyBlue1,         "SkyBlue1"/*#87d7ff*/, 0x87, 0xd7, 0xff},//#0x87,0xd7,0xff	rgb(135,215,255)	hsl(200,100%,76%)
    {Color::Chartreuse1,      "Chartreuse1"/*#87f*/, 0x87, 0xff, 0x00},//#87ff00	rgb(135,255,0)	hsl(8,100%,50%)
    {Color::LightGreen,       "LightGreen"/*#87ff*/, 0x87, 0xff, 0x5f},//#87ff5f	rgb(135,255,95)	hsl(105,100%,68%)
    {Color::LightGreen2,      "LightGreen2"/*#87f*/, 0x87, 0xff, 0x87},//#87ff87	rgb(135,255,135)	hsl(120,100%,76%)
    {Color::PaleGreen1,       "PaleGreen1"/*#87ff*/, 0x87, 0xff, 0xaf},//#87ffaf	rgb(135,255,175)	hsl(140,100%,76%)
    {Color::Aquamarine2,      "Aquamarine2"/*#87f*/, 0x87, 0xff, 0xd7},//#87ffd7	rgb(135,255,215)	hsl(160,100%,76%)
    {Color::DarkSlateGray1,   "DarkSlateGray1"/*#*/, 0x87, 0xff, 0xff},//#87ffff	rgb(135,255,255)	hsl(180,100%,76%)
    {Color::Red3,             "Red3"/*#af0000	*/, 0xaf, 0x00, 0x00},//#af0000	rgb(175,0,0)	hsl(0,100%,34%)
    {Color::DeepPink6,        "DeepPink6"/*#af005*/, 0xaf, 0x00, 0x5f},//#af005f	rgb(175,0,95)	hsl(27,100%,34%)
    {Color::MediumVioletRed,  "MediumVioletRed"/**/, 0xaf, 0x00, 0x87},//#af0087	rgb(175,0,135)	hsl(13,100%,34%)
    {Color::Magenta3,         "Magenta3"/*#af00af*/, 0xaf, 0x00, 0xaf},//#af00af	rgb(175,0,175)	hsl(300,100%,34%)
    {Color::DarkViole2,       "DarkViole2"/*#af00*/, 0xaf, 0x00, 0xd7},//#af00d7	rgb(175,0,215)	hsl(88,100%,42%)
    {Color::Purple6,          "Purple6"/*#af00ff	*/, 0xaf, 0x00, 0xff},//#af00ff	rgb(175,0,255)	hsl(81,100%,50%)
    {Color::DarkOrange3,      "DarkOrange3"/*#af5*/, 0xaf, 0x5f, 0x00},//#af5f00	rgb(175,95,0)	hsl(2,100%,34%)
    {Color::IndianRed,        "IndianRed"/*#af5f5*/, 0xaf, 0x5f, 0x5f},//#af5f5f	rgb(175,95,95)	hsl(0,33%,52%)
    {Color::HotPink3,         "HotPink3"/*#af5f87*/, 0xaf, 0x5f, 0x87},//#af5f87	rgb(175,95,135)	hsl(330,33%,52%)
    {Color::MediumOrchid3,    "MediumOrchid3"/*#A*/, 0xaf, 0x5f, 0xaf},//#af5faf	rgb(175,95,175)	hsl(300,33%,52%)
    {Color::MediumOrchid,     "MediumOrchid"/*#af*/, 0xaf, 0x5f, 0xd7},//#af5fd7	rgb(175,95,215)	hsl(280,60%,60%)
    {Color::MediumPurple2,    "MediumPurple2"/*#A*/, 0xaf, 0x5f, 0xff},//#af5fff	rgb(175,95,255)	hsl(270,100%,68%)
    {Color::DarkGoldenrod,    "DarkGoldenrod"/*#A*/, 0xaf, 0x87, 0x00},//#af8700	rgb(175,135,0)	hsl(6,100%,34%)
    {Color::LightSalmon3,     "LightSalmon3"/*#af*/, 0xaf, 0x87, 0x5f},//#af875f	rgb(175,135,95)	hsl(30,33%,52%)
    {Color::RosyBrown,        "RosyBrown"/*#af878*/, 0xaf, 0x87, 0x87},//#af8787	rgb(175,135,135)	hsl(0,20%,60%)
    {Color::Grey63,           "Grey63"/*#af87af	*/, 0xaf, 0x87, 0xaf},//#af87af	rgb(175,135,175)	hsl(300,20%,60%)
    {Color::MediumPurple6,    "MediumPurple6"/*#A*/, 0xaf, 0x87, 0xd7},//#af87d7	rgb(175,135,215)	hsl(270,50%,68%)
    {Color::MediumPurple1,    "MediumPurple1"/*#A*/, 0xaf, 0x87, 0xff},//#af87ff	rgb(175,135,255)	hsl(260,100%,76%)
    {Color::Gold3,            "Gold3"/*#afaf00	*/, 0xaf, 0xaf, 0x00},//#afaf00	rgb(175,175,0)	hsl(60,100%,34%)
    {Color::DarkKhaki,        "DarkKhaki"/*#afaf5*/, 0xaf, 0xaf, 0x5f},//#afaf5f	rgb(175,175,95)	hsl(60,33%,52%)
    {Color::NavajoWhite3,     "NavajoWhite3"/*#af*/, 0xaf, 0xaf, 0x87},//#afaf87	rgb(175,175,135)	hsl(60,20%,60%)
    {Color::Grey69,           "Grey69"/*#afafaf	*/, 0xaf, 0xaf, 0xaf},//#afafaf	rgb(175,175,175)	hsl(0,0%,68%)
    {Color::LightSteelBlue3,  "LightSteelBlue3"/**/, 0xaf, 0xaf, 0xd7},//#afafd7	rgb(175,175,215)	hsl(240,33%,76%)
    {Color::LightSteelBlue,   "LightSteelBlue"/*#*/, 0xaf, 0xaf, 0xff},//#afafff	rgb(175,175,255)	hsl(240,100%,84%)
    {Color::Yellow3,          "Yellow3"/*#afd700	*/, 0xaf, 0xd7, 0x00},//#afd700	rgb(175,215,0)	hsl(1,100%,42%)
    {Color::DarkOliveGreen5,  "DarkOliveGreen5"/**/, 0xaf, 0xd7, 0x5f},//#afd75f	rgb(175,215,95)	hsl(80,60%,60%)
    {Color::DarkSeaGreen6,    "DarkSeaGreen6"/*#A*/, 0xaf, 0xd7, 0x87},//#afd787	rgb(175,215,135)	hsl(90,50%,68%)
    {Color::DarkSeaGreen2,    "DarkSeaGreen2"/*#A*/, 0xaf, 0xd7, 0xaf},//#afd7af	rgb(175,215,175)	hsl(120,33%,76%)
    {Color::LightCyan3,       "LightCyan3"/*#afd7*/, 0xaf, 0xd7, 0xd7},//#afd7d7	rgb(175,215,215)	hsl(180,33%,76%)
    {Color::LightSkyBlue1,    "LightSkyBlue1"/*#A*/, 0xaf, 0xd7, 0xff},//#afd7ff	rgb(175,215,255)	hsl(210,100%,84%)
    {Color::GreenYellow,      "GreenYellow"/*#aff*/, 0xaf, 0xff, 0x00},//#afff00	rgb(175,255,0)	hsl(8,100%,50%)
    {Color::DarkOliveGreen2,  "DarkOliveGreen2"/**/, 0xaf, 0xff, 0x5f},//#afff5f	rgb(175,255,95)	hsl(90,100%,68%)
    {Color::PaleGreen2,       "PaleGreen2"/*#afff*/, 0xaf, 0xff, 0x87},//#afff87	rgb(175,255,135)	hsl(100,100%,76%)
    {Color::DarkSeaGreen7,    "DarkSeaGreen7"/*#A*/, 0xaf, 0xff, 0xaf},//#afffaf	rgb(175,255,175)	hsl(120,100%,84%)
    {Color::DarkSeaGreen1,    "DarkSeaGreen1"/*#A*/, 0xaf, 0xff, 0xd7},//#afffd7	rgb(175,255,215)	hsl(150,100%,84%)
    {Color::PaleTurquoise1,   "PaleTurquoise1"/*#*/, 0xaf, 0xff, 0xff},//#afffff	rgb(175,255,255)	hsl(180,100%,84%)
    {Color::Red4,             "Red4"/*#d70000	*/, 0xd7, 0x00, 0x00},//#d70000	rgb(215,0,0)	hsl(0,100%,42%)
    {Color::DeepPink3,        "DeepPink3"/*#d7005*/, 0xd7, 0x00, 0x5f},//#d7005f	rgb(215,0,95)	hsl(33,100%,42%)
    {Color::DeepPink7,        "DeepPink7"/*#d7008*/, 0xd7, 0x00, 0x87},//#d70087	rgb(215,0,135)	hsl(22,100%,42%)
    {Color::Magenta4,         "Magenta4"/*#d700af*/, 0xd7, 0x00, 0xaf},//#d700af	rgb(215,0,175)	hsl(11,100%,42%)
    {Color::Magenta5,         "Magenta5"/*#d700d7*/, 0xd7, 0x00, 0xd7},//#d700d7	rgb(215,0,215)	hsl(300,100%,42%)
    {Color::Magenta2,         "Magenta2"/*#d700ff*/, 0xd7, 0x00, 0xff},//#d700ff	rgb(215,0,255)	hsl(90,100%,50%)
    {Color::DarkOrange4,      "DarkOrange4"/*#d75*/, 0xd7, 0x5f, 0x00},//#d75f00	rgb(215,95,0)	hsl(6,100%,42%)
    {Color::IndianRed2,       "IndianRed2"/*#d75f*/, 0xd7, 0x5f, 0x5f},//#d75f5f	rgb(215,95,95)	hsl(0,60%,60%)
    {Color::HotPink4,         "HotPink4"/*#d75f87*/, 0xd7, 0x5f, 0x87},//#d75f87	rgb(215,95,135)	hsl(340,60%,60%)
    {Color::HotPink2,         "HotPink2"/*#d75faf*/, 0xd7, 0x5f, 0xaf},//#d75faf	rgb(215,95,175)	hsl(320,60%,60%)
    {Color::Orchid,           "Orchid"/*#d75fd7	*/, 0xd7, 0x5f, 0xd7},//#d75fd7	rgb(215,95,215)	hsl(300,60%,60%)
    {Color::MediumOrchid1,    "MediumOrchid1"/*#d*/, 0xd7, 0x5f, 0xff},//#d75fff	rgb(215,95,255)	hsl(285,100%,68%)
    {Color::Orange3,          "Orange3"/*#d78700	*/, 0xd7, 0x87, 0x00},//#d78700	rgb(215,135,0)	hsl(7,100%,42%)
    {Color::LightSalmon4,     "LightSalmon4"/*#d7*/, 0xd7, 0x87, 0x5f},//#d7875f	rgb(215,135,95)	hsl(20,60%,60%)
    {Color::LightPink3,       "LightPink3"/*#d787*/, 0xd7, 0x87, 0x87},//#d78787	rgb(215,135,135)	hsl(0,50%,68%)
    {Color::Pink3,            "Pink3"/*#d787af	*/, 0xd7, 0x87, 0xaf},//#d787af	rgb(215,135,175)	hsl(330,50%,68%)
    {Color::Plum3,            "Plum3"/*#d787d7	*/, 0xd7, 0x87, 0xd7},//#d787d7	rgb(215,135,215)	hsl(300,50%,68%)
    {Color::Violet,           "Violet"/*#d787ff	*/, 0xd7, 0x87, 0xff},//#d787ff	rgb(215,135,255)	hsl(280,100%,76%)
    {Color::Gold4,            "Gold4"/*#d7af00	*/, 0xd7, 0xaf, 0x00},//#d7af00	rgb(215,175,0)	hsl(8,100%,42%)
    {Color::LightGoldenrod3,  "LightGoldenrod3"/**/, 0xd7, 0xaf, 0x5f},//#d7af5f	rgb(215,175,95)	hsl(40,60%,60%)
    {Color::Tan,              "Tan"/*#d7af87	rgb(*/, 0xd7, 0xaf, 0x87},//#d7af87	rgb(215,175,135)	hsl(30,50%,68%)
    {Color::MistyRose3,       "MistyRose3"/*#d7af*/, 0xd7, 0xaf, 0xaf},//#d7afaf	rgb(215,175,175)	hsl(0,33%,76%)
    {Color::Thistle3,         "Thistle3"/*#d7afd7*/, 0xd7, 0xaf, 0xd7},//#d7afd7	rgb(215,175,215)	hsl(300,33%,76%)
    {Color::Plum2,            "Plum2"/*#d7afff	*/, 0xd7, 0xaf, 0xff},//#d7afff	rgb(215,175,255)	hsl(270,100%,84%)
    {Color::Yellow6,          "Yellow6"/*#d7d700	*/, 0xd7, 0xd7, 0x00},//#d7d700	rgb(215,215,0)	hsl(60,100%,42%)
    {Color::Khaki3,           "Khaki3"/*#d7d75f	*/, 0xd7, 0xd7, 0x5f},//#d7d75f	rgb(215,215,95)	hsl(60,60%,60%)
    {Color::LightGoldenrod2,  "LightGoldenrod2"/**/, 0xd7, 0xd7, 0x87},//#d7d787	rgb(215,215,135)	hsl(60,50%,68%)
    {Color::LightYellow3,     "LightYellow3"/*#d7*/, 0xd7, 0xd7, 0xaf},//#d7d7af	rgb(215,215,175)	hsl(60,33%,76%)
    {Color::Grey84,           "Grey84"/*#d7d7d7	*/, 0xd7, 0xd7, 0xd7},//#d7d7d7	rgb(215,215,215)	hsl(0,0%,84%)
    {Color::LightSteelBlue1,  "LightSteelBlue1"/**/, 0xd7, 0xd7, 0xff},//#d7d7ff	rgb(215,215,255)	hsl(240,100%,92%)
    {Color::Yellow2,          "Yellow2"/*#d7ff00	*/, 0xd7, 0xff, 0x00},//#d7ff00	rgb(215,255,0)	hsl(9,100%,50%)
    {Color::DarkOliveGreen1,  "DarkOliveGreen1"/**/, 0xd7, 0xff, 0x5f},//#d7ff5f	rgb(215,255,95)	hsl(75,100%,68%)
    {Color::DarkOliveGreen6,  "DarkOliveGreen6"/**/, 0xd7, 0xff, 0x87},//#d7ff87	rgb(215,255,135)	hsl(80,100%,76%)
    {Color::DarkSeaGreen8,    "DarkSeaGreen8"/*#d*/, 0xd7, 0xff, 0xaf},//#d7ffaf	rgb(215,255,175)	hsl(90,100%,84%)
    {Color::Honeydew2,        "Honeydew2"/*#d7ffd*/, 0xd7, 0xff, 0xd7},//#d7ffd7	rgb(215,255,215)	hsl(120,100%,92%)
    {Color::LightCyan1,       "LightCyan1"/*#d7ff*/, 0xd7, 0xff, 0xff},//#d7ffff	rgb(215,255,255)	hsl(180,100%,92%)
    {Color::Red1,             "Red1"/*#ff0000	*/, 0xff, 0x00, 0x00},//#ff0000	rgb(255,0,0)	hsl(0,100%,50%)
    {Color::DeepPink2,        "DeepPink2"/*#ff005*/, 0xff, 0x00, 0x5f},//#ff005f	rgb(255,0,95)	hsl(37,100%,50%)
    {Color::DeepPink1,        "DeepPink1"/*#ff008*/, 0xff, 0x00, 0x87},//#ff0087	rgb(255,0,135)	hsl(28,100%,50%)
    {Color::DeepPink8,        "DeepPink8"/*#ff00a*/, 0xff, 0x00, 0xaf},//#ff00af	rgb(255,0,175)	hsl(18,100%,50%)
    {Color::Magenta6,         "Magenta6"/*#ff00d7*/, 0xff, 0x00, 0xd7},//#ff00d7	rgb(255,0,215)	hsl(09,100%,50%)
    {Color::Magenta1,         "Magenta1"/*#ff00ff*/, 0xff, 0x00, 0xff},//#ff00ff	rgb(255,0,255)	hsl(300,100%,50%)
    {Color::OrangeRed1,       "OrangeRed1"/*#ff5f*/, 0xff, 0x5f, 0x00},//#ff5f00	rgb(255,95,0)	hsl(2,100%,50%)
    {Color::IndianRed1,       "IndianRed1"/*#ff5f*/, 0xff, 0x5f, 0x5f},//#ff5f5f	rgb(255,95,95)	hsl(0,100%,68%)
    {Color::IndianRed3,       "IndianRed3"/*#ff5f*/, 0xff, 0x5f, 0x87},//#ff5f87	rgb(255,95,135)	hsl(345,100%,68%)
    {Color::HotPink,          "HotPink"/*#ff5faf	*/, 0xff, 0x5f, 0xaf},//#ff5faf	rgb(255,95,175)	hsl(330,100%,68%)
    {Color::HotPin2,          "HotPin2"/*#ff5fd7	*/, 0xff, 0x5f, 0xd7},//#ff5fd7	rgb(255,95,215)	hsl(315,100%,68%)
    {Color::MediumOrchid2,    "MediumOrchid2"/*#f*/, 0xff, 0x5f, 0xff},//#ff5fff	rgb(255,95,255)	hsl(300,100%,68%)
    {Color::DarkOrange,       "DarkOrange"/*#ff87*/, 0xff, 0x87, 0x00},//#ff8700	rgb(255,135,0)	hsl(1,100%,50%)
    {Color::Salmon1,          "Salmon1"/*#ff875f	*/, 0xff, 0x87, 0x5f},//#ff875f	rgb(255,135,95)	hsl(15,100%,68%)
    {Color::LightCoral,       "LightCoral"/*#ff87*/, 0xff, 0x87, 0x87},//#ff8787	rgb(255,135,135)	hsl(0,100%,76%)
    {Color::PaleVioletRed1,   "PaleVioletRed1"/*#*/, 0xff, 0x87, 0xaf},//#ff87af	rgb(255,135,175)	hsl(340,100%,76%)
    {Color::Orchid2,          "Orchid2"/*#ff87d7	*/, 0xff, 0x87, 0xd7},//#ff87d7	rgb(255,135,215)	hsl(320,100%,76%)
    {Color::Orchid1,          "Orchid1"/*#ff87ff	*/, 0xff, 0x87, 0xff},//#ff87ff	rgb(255,135,255)	hsl(300,100%,76%)
    {Color::Orange1,          "Orange1"/*#ffaf00	*/, 0xff, 0xaf, 0x00},//#ffaf00	rgb(255,175,0)	hsl(1,100%,50%)
    {Color::SandyBrown,       "SandyBrown"/*#ffaf*/, 0xff, 0xaf, 0x5f},//#ffaf5f	rgb(255,175,95)	hsl(30,100%,68%)
    {Color::LightSalmon1,     "LightSalmon1"/*#ff*/, 0xff, 0xaf, 0x87},//#ffaf87	rgb(255,175,135)	hsl(20,100%,76%)
    {Color::LightPink1,       "LightPink1"/*#ffaf*/, 0xff, 0xaf, 0xaf},//#ffafaf	rgb(255,175,175)	hsl(0,100%,84%)
    {Color::Pink1,            "Pink1"/*#ffafd7	*/, 0xff, 0xaf, 0xd7},//#ffafd7	rgb(255,175,215)	hsl(330,100%,84%)
    {Color::Plum1,            "Plum1"/*#ffafff	*/, 0xff, 0xaf, 0xff},//#ffafff	rgb(255,175,255)	hsl(300,100%,84%)
    {Color::Gold1,            "Gold1"/*#ffd700	*/, 0xff, 0xd7, 0x00},//#ffd700	rgb(255,215,0)	hsl(0,100%,50%)
    {Color::LightGoldenrod4,  "LightGoldenrod4"/**/, 0xff, 0xd7, 0x5f},//#ffd75f	rgb(255,215,95)	hsl(45,100%,68%)
    {Color::LightGoldenrod5,  "LightGoldenrod5"/**/, 0xff, 0xd7, 0x87},//#ffd787	rgb(255,215,135)	hsl(40,100%,76%)
    {Color::NavajoWhite1,     "NavajoWhite1"/*#ff*/, 0xff, 0xd7, 0xaf},//#ffd7af	rgb(255,215,175)	hsl(30,100%,84%)
    {Color::MistyRose1,       "MistyRose1"/*#ffd7*/, 0xff, 0xd7, 0xd7},//#ffd7d7	rgb(255,215,215)	hsl(0,100%,92%)
    {Color::Thistle1,         "Thistle1"/*#ffd7ff*/, 0xff, 0xd7, 0xff},//#ffd7ff	rgb(255,215,255)	hsl(300,100%,92%)
    {Color::Yellow1,          "Yellow1"/*#ffff00	*/, 0xff, 0xff, 0x00},//#ffff00	rgb(255,255,0)	hsl(60,100%,50%)
    {Color::LightGoldenrod1,  "LightGoldenrod1"/**/, 0xff, 0xff, 0x5f},//#ffff5f	rgb(255,255,95)	hsl(60,100%,68%)
    {Color::Khaki1,           "Khaki1"/*#ffff87	*/, 0xff, 0xff, 0x87},//#ffff87	rgb(255,255,135)	hsl(60,100%,76%)
    {Color::Wheat1,           "Wheat1"/*#ffffaf	*/, 0xff, 0xff, 0xaf},//#ffffaf	rgb(255,255,175)	hsl(60,100%,84%)
    {Color::Cornsilk1,        "Cornsilk1"/*#ffffd*/, 0xff, 0xff, 0xd7},//#ffffd7	rgb(255,255,215)	hsl(60,100%,92%)
    {Color::Grey100,          "Grey100"/*#ffffff	*/, 0xff, 0xff, 0xff},//#ffffff	rgb(255,255,255)	hsl(0,0%,100%)
    {Color::Grey3,            "Grey3"/*#080808	*/, 0x08, 0x08, 0x08},//#080808	rgb(8,8,8)	hsl(0,0%,3%)
    {Color::Grey7,            "Grey7"/*#121212	*/, 0x12, 0x12, 0x12},//#121212	rgb(18,18,18)	hsl(0,0%,7%)
    {Color::Grey11,           "Grey11"/*#1c1c1c	*/, 0x1c, 0x1c, 0x1c},//#1c1c1c	rgb(28,28,28)	hsl(0,0%,10%)
    {Color::Grey15,           "Grey15"/*#262626	*/, 0x26, 0x26, 0x26},//#262626	rgb(38,38,38)
    {Color::Grey19,           "Grey19"/*#303030	*/, 0x30, 0x30, 0x30},//#303030	rgb(48,48,48)	hsl(0,0%,18%)
    {Color::Grey23,           "Grey23"/*#3a3a3a	*/, 0x3a, 0x3a, 0x3a},//#3a3a3a	rgb(58,58,58)	hsl(0,0%,22%)
    {Color::Grey27,           "Grey27"/*#444444	*/, 0x44, 0x44, 0x44},//#444444	rgb(68,68,68)	hsl(0,0%,26%)
    {Color::Grey30,           "Grey30"/*#4e4e4e	*/, 0x4e, 0x4e, 0x4e},//#4e4e4e	rgb(78,78,78)	hsl(0,0%,30%)
    {Color::Grey35,           "Grey35"/*#585858	*/, 0x58, 0x58, 0x58},//#585858	rgb(88,88,88)	hsl(0,0%,34%)
    {Color::Grey39,           "Grey39"/*#626262	*/, 0x62, 0x62, 0x62},//#626262	rgb(98,98,98)	hsl(0,0%,37%)
    {Color::Grey42,           "Grey42"/*#6c6c6c	*/, 0x6c, 0x6c, 0x6c},//#6c6c6c	rgb(108,108,108)	hsl(0,0%,40%)
    {Color::Grey46,           "Grey46"/*#767676	*/, 0x76, 0x76, 0x76},//#767676	rgb(118,118,118)	hsl(0,0%,46%)
    {Color::Grey50,           "Grey50"/*#808080	*/, 0x80, 0x80, 0x80},//#808080	rgb(128,128,128)	hsl(0,0%,50%)
    {Color::Grey54,           "Grey54"/*#8a8a8a	*/, 0x8a, 0x8a, 0x8a},//#8a8a8a	rgb(138,138,138)	hsl(0,0%,54%)
    {Color::Grey58,           "Grey58"/*#949494	*/, 0x94, 0x94, 0x94},//#949494	rgb(148,148,148)	hsl(0,0%,58%)
    {Color::Grey62,           "Grey62"/*#9e9e9e	*/, 0x9e, 0x9e, 0x9e},//#9e9e9e	rgb(158,158,158)	hsl(0,0%,61%)
    {Color::Grey66,           "Grey66"/*#a8a8a8	*/, 0xa8, 0xa8, 0xa8},//#a8a8a8	rgb(168,168,168)	hsl(0,0%,65%)
    {Color::Grey70,           "Grey70"/*#b2b2b2	*/, 0xb2, 0xb2, 0xb2},//#b2b2b2	rgb(178,178,178)	hsl(0,0%,69%)
    {Color::Grey74,           "Grey74"/*#bcbcbc	*/, 0xbc, 0xbc, 0xbc},//#bcbcbc	rgb(188,188,188)	hsl(0,0%,73%)
    {Color::Grey78,           "Grey78"/*#c6c6c6	*/, 0xc6, 0xc6, 0xc6},//#c6c6c6	rgb(198,198,198)	hsl(0,0%,77%)
    {Color::Grey82,           "Grey82"/*#d0d0d0	*/, 0xd0, 0xd0, 0xd0},//#d0d0d0	rgb(208,208,208)	hsl(0,0%,81%)
    {Color::Grey85,           "Grey85"/*#dadada	*/, 0xda, 0xda, 0xda},//#dadada	rgb(218,218,218)	hsl(0,0%,85%)
    {Color::Grey89,           "Grey89"/*#e4e4e4	*/, 0xe4, 0xe4, 0xe4},//#e4e4e4	rgb(228,228,228)	hsl(0,0%,89%)
    {Color::Reset,            "Reset"/*#eeeeee	*/, 0,    0,    0}
};



/*!
 * @brief Converts Color::Code Fg To console Ansi string command
 * @param Fg
 * @return std::string
 */
std::string Color::Ansi(Color::Code Fg)
{
    if (Fg == Color::Reset) { return "\033[0m"; }
    std::ostringstream Out;
    Out << "\033[38;5;" << static_cast<int>(Fg) << 'm';

    return Out.str();
}

std::string Color::Ansi(Color::Pair Pair)
{
    std::ostringstream Out;
    Out << "\033[";
    if ((Pair.Fg == Color::Reset) && (Pair.Bg == Color::Reset)) Out << "0";
    if(Pair.Fg != Color::Reset)  Out << "38;5;" << static_cast<int>(Pair.Fg);
    if(Pair.Bg != Color::Reset) Out <<  ";48;5;" << static_cast<int>(Pair.Bg);
    Out << 'm';

    return Out.str();
}

std::string Color::AnsiBg(Color::Code Bg)
{
    if (Bg == Color::Reset) { return "\033[0m"; }
    std::ostringstream Out;
    Out << "\033[48;5;" << static_cast<int>(Bg) << 'm';

    return Out.str();
}

Color::Code Color::Scan(const std::string &ColName)
{
    int N = 0;
    for (auto& Clr : ColorDB)
    {
        if (ColName == Clr.ColorName) return static_cast<Color::Code>(N);
        ++N;
    }
    return Color::Reset;
}

std::string Color::Name(Color::Code Clr)
{
    return ColorDB[Clr].ColorName.data();
}

std::string Color::Html(Color::Code Clr)
{
    return "0";
}

std::string Color::Html(Color::Pair Pair)
{
    return "0";
}

Color::Data Color::Argb(Color::Code A)
{
    return ColorDB[static_cast<size_t>(A)];
}


std::string Color::Item::operator()()
{
    return Data();
}

Color::Item &Color::Item::operator>>(std::string &Out)
{
    Data >> Out;
    return *this;
}

Color::Item &Color::Array::operator[](const std::string &ItemID)
{
    for (auto& item : Items)
        if (item.Id == ItemID) return item;
    return *Items.begin(); // Toujours mettre default values au debut...
}

std::string Color::Pair::operator()() const
{
    return Color::Ansi(*this);
}

Color::Pair &Color::Pair::operator>>(std::string &out)
{
    out = Color::Ansi(*this);
    return *this;
}
//} // Book