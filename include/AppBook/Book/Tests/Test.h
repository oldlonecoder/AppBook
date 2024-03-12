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
#include "AppBook/Util/CArgs.h"


//#ifndef APPBOOK_TEST_H
//#define APPBOOK_TEST_H

namespace Book
{

/*!
* @brief Base class of the AppBook Tests API.
 *
 * Note The Goal of this is to get and produce something like this: (examples)
 * @code

----------------------------------------------------
Test #1 "Book::StrBreak" |  PASSED   👍  / FAILED ✘
----------------------------------------------------
Test #2 "Book::STMLText" |  PASSED   👍  / FAILED ✘
----------------------------------------------------
Test #3 "Util::CArgs"    |  PASSED   👍  / FAILED ✘
----------------------------------------------------
 //...
@endcode

 Structure of Util::Object <- Book::Test :
 ---------------------------------------------------------------
 Test ID Title  | {Input -> Output} data | Status/Result
 ---------------|------------------------|----------------------

 ---------------------------------------------------------------

 - Assert
 - Expect

 */
class APPBOOK_EXPORTS Test : public Util::Object
{

    int8_t              mTestNum{0};

public:


    enum ConfigOptions
    {
    //???
    };

    Test() = default;
    Test(Test* ParentTest, const std::string& Name);
    ~Test() override;

    virtual Book::Result Run() = 0;
    void Passed();
    void Failed();

};

} // Book

//#endif //APPBOOK_TEST_H
