//
// Created by oldlonecoder on 24-01-29.
//

#include "AppBook/Book/Tests/Test.h"

namespace Book
{


Test::Test(Test *ParentTest, const std::string &Name): Util::Object(ParentTest, Name) {}

Test::~Test(){}

void Test::Passed()
{
    //...
    AppBook::Test() << Color::White << Id() << Color::Reset << " | " <<  Color::LightGreen2 << Utf::Glyph::Success << " " << Color::Green4 << "Passed "<< Color::Reset;
    //...
}

void Test::Failed()
{
    //...
    AppBook::Test() << Color::White << Id() << Color::Reset << " | "<< Color::Red1 << Utf::Glyph::Err1 << " " << Color::Red4 << "Failed " << Color::Reset;
    //...
}


} // Book