/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once
#include "AppBook/Util/Object.h"
#include "AppBook/Util/StrAcc.h"
#include "AppBook/Util/Geometry.h"
#include "AppBook/Utf/Glyphes.h"
#include "AppBook/Book/BookEnums.h"

#include <filesystem>
#include <fstream>
#include <memory> //< Check if a smart pointer could delete Book::ApplicationBook properly.

#include <map>




/*!
 * \brief The Book class



 * \author &copy;2023, oldlonecoder
 */

class APPBOOK_EXPORTS AppBook : public Util::Object {

    std::ostream *out_stream{nullptr}; ///< Pointer to the address of the currently selected output stream.
    Color::Format _Format{Color::Format::ansi256};
    std::string starting_path;

public:

    struct APPBOOK_EXPORTS ElementComponents
    {
        uint16_t icon: 1;
        uint16_t filename: 1;
        uint16_t line: 1;
        uint16_t column: 1;
        uint16_t funcname: 1;
        uint16_t funcsig: 1;
        uint16_t stamp: 1;
        uint16_t time: 1;
        uint16_t day: 1;
        uint16_t month: 1;
        uint16_t year: 1;
        //...
        using Dictionary = std::map<std::string, AppBook::ElementComponents>;
    };

    static AppBook::ElementComponents::Dictionary ComponentsHeaders;


    struct APPBOOK_EXPORTS config_data
    {
        std::string Filename;
        [[maybe_unused]] Color::Format OutputFormat;
        [[maybe_unused]] bool Immediate;
    };

    /*!
     * \brief The Section class
    */
    struct APPBOOK_EXPORTS Section : public Object
    {

    using Array = std::vector<AppBook::Section *>;
    using Iterator [[maybe_unused]] = Array::iterator;

    /*!
    * \brief The bloc class
    *
    * \code // ex.:
    *
    *    //...
    *    Book::bloc* aro = Book::instance["Runtime"]["arithmetics-dev"];
    *    //...
    *    if(alu_fail)
    *        throw aro.except(HERE) << " alu: division by zero";
    *    //...
    *    aro.debug(HERE)(.../xio.cc line 340; function: collapse_closepair( xio* ) ) << " debug trace...";
    *
    *    //...
    *    Book::select("Section-id/bloc-id");
    *    ----------------------------------------- :>
    *
    *
    *
    *    // ...
    *
    *    Book::out() << " salutation!";
    *
    *
    *
    */
    struct APPBOOK_EXPORTS Contents : public Object
    {

        std::ofstream OutputFile; ///< the Filename and path Location are implicitely set by the parent Section, this bloc ID and the ouput format.
        std::string Descriptions;
        std::string Filename;
        std::filesystem::path Location;
        using Array = std::vector<AppBook::Section::Contents *>;
        using Iterator = Array::iterator;
        size_t IndentationLevel{0};
        size_t IndentationValue{4};


        Book::Enums::Code Commit(); ///< push element compiled STMLText content into the output stream.

        std::string GetFilename();


        struct APPBOOK_EXPORTS Element : public Object
        {
            using Memory = std::vector<AppBook::Section::Contents::Element>;
            std::vector<std::string> InputComponents;
            StrAcc Text;
            Book::Enums::Class Class{Book::Enums::Class::None};
            Book::Enums::Code Code{Book::Enums::Code::Rejected};
            std::source_location Src{};
            AppBook::ElementComponents Ec{0};

            Element() = default;
            Element(const Element &) = default;
            Element(Element && e) noexcept = default;
            Element(Object* Par, Book::Enums::Class aClass, std::source_location aSrc);

            template<typename T> Book::Enums::Code Input(T const& Arg)
            {
                if(!InputComponents.empty())
                {
                    StrAcc Acc = InputComponents.back();
                    if(Acc.ScanArg() != std::string::npos)
                    {
                        Acc << Arg;
                        Text << Acc;
                        InputComponents.back() = Text();
                        Text.Clear();
                        return Book::Enums::Code::Implemented;
                    }
                }
                Text <<Arg;
                InputComponents.push_back(Text());
                Text.Clear();
                return Book::Enums::Code::Ok;
            }

            AppBook::Section::Contents::Element &operator=(const AppBook::Section::Contents::Element &e) = default;
            AppBook::Section::Contents::Element &operator=(AppBook::Section::Contents::Element && e) noexcept = default;
            AppBook::Section::Contents::Element &operator<<(Utf::Glyph::Type graphem);
            AppBook::Section::Contents::Element &operator<<(Utf::AccentFR::Type accent);
            AppBook::Section::Contents::Element &operator<<(const StrAcc &txt);
            AppBook::Section::Contents::Element &operator<<(const std::string &txt);
            AppBook::Section::Contents::Element &operator<<(const char *txt);
            AppBook::Section::Contents::Element &operator<<(char c);
            AppBook::Section::Contents::Element &operator<<(Book::Enums::Code c);
            AppBook::Section::Contents::Element &operator<<(Book::Enums::Class c);
            AppBook::Section::Contents::Element &operator<<(Color::Code c);
            AppBook::Section::Contents::Element &operator<<(const Rect &R);
            AppBook::Section::Contents::Element &operator<<(Book::Enums::Action A);
            AppBook::Section::Contents::Element &operator<<(Book::Enums::Fn tr);
            AppBook::Section::Contents::Element &operator<<(AppBook::ElementComponents cfg);
            AppBook::Section::Contents::Element & operator << (Point Pt);
            AppBook::Section::Contents::Element & operator << (Dim WH);
            AppBook::Section::Contents::Element & operator << (Rect R);
            template<typename other_types> AppBook::Section::Contents::Element &operator<<(other_types val)
            {
                Input(val);
                return *this;
            }
            
            void CompileHeader();
            Book::Enums::Code Compile();
            Book::Enums::Code Commit();
        };

        Contents() = default;

        AppBook::Section::Contents::Element::Memory MemoryStream;
        Contents(Object* parent_obj /* Normally AppBook::Section* */, const std::string &atitle);

        ~Contents() override;

        Book::Enums::Code Open();///< If exist, open in append mode. or create if not exist.
        Book::Enums::Code Close();
        
        AppBook::Section::Contents::Element &Error(std::source_location src);
        AppBook::Section::Contents::Element &Out(std::source_location src);
        AppBook::Section::Contents::Element &Warning(std::source_location src);
        AppBook::Section::Contents::Element &Fatal(std::source_location src);
        AppBook::Section::Contents::Element &Except(std::source_location src);
        AppBook::Section::Contents::Element &Message(std::source_location src);
        AppBook::Section::Contents::Element &Debug(std::source_location src);
        AppBook::Section::Contents::Element &Info(std::source_location src);
        AppBook::Section::Contents::Element &Comment(std::source_location src);
        AppBook::Section::Contents::Element &Syntax(std::source_location src);
        AppBook::Section::Contents::Element &Status(std::source_location src);
        AppBook::Section::Contents::Element &Test(std::source_location src);
        AppBook::Section::Contents::Element &Interrupted(std::source_location src);
        AppBook::Section::Contents::Element &Aborted(std::source_location src);
        AppBook::Section::Contents::Element &Segfault(std::source_location src);

    };

    std::string Title;
    std::string Description;

    std::filesystem::path Location;


    Section(Object* par, const std::string &section_id);
    ~Section() override;

    Book::Enums::Code Close();
    AppBook::Section &Open();
    AppBook::Section::Contents &CreateSectionContents(const std::string &aContentID);

    Contents::Array Blocs;

    AppBook::Section::Contents &operator[](const std::string &aContentsID);

    };

    class APPBOOK_EXPORTS Exception :  public std::exception
    {
    public:

        explicit Exception():std::exception() { E = true; }
        explicit Exception(const char *txt) { msg = txt; E = false; }
        explicit Exception(const std::string &txt) { msg = txt; E = false; }

        ~Exception() noexcept override {
            msg.clear();
        }

        [[nodiscard]] const char *what() const noexcept override;

        AppBook::Exception operator[](AppBook::Section::Contents::Element El);
    private:
        bool E;
        std::string msg;
        AppBook::Section::Contents::Element Element;
    };


    ~AppBook() override;
    config_data &config(){ return Conf; }
    static AppBook& Open(const std::string& BookName);
    static AppBook& Begin(const std::string& BookName);
    static Book::Enums::Code Close();
    static Book::Enums::Code End();
    static AppBook& Instance();
    std::string Descriptions;

    AppBook::Section &operator[](std::string_view section_id);
    static AppBook::Section &CreateSection(const std::string &section_id);
    static std::stack<AppBook::Section::Contents*> ContextStack;
    static Color::Format Format();

    static void ThrowOnNoStream();
    static AppBook &Self();
    static Book::Result PushContext();
    static Book::Result PopContext();


    [[maybe_unused]] static std::filesystem::path LocationPath;
    [[maybe_unused]] static AppBook::Section::Contents::Element &Error(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Out(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Warning(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Fatal(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Except(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Message(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Debug(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Info(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Comment(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Syntax(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Status(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Test(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Interrupted(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Aborted(std::source_location src = std::source_location::current());
    [[maybe_unused]] static AppBook::Section::Contents::Element &Segfault(std::source_location src = std::source_location::current());
    [[maybe_unused]] static Book::Enums::Code Commit();

//    --- held back ...
//    static std::string today();
//    static std::string date();
//    static std::string time(const std::string& tz="America/Toronto"); ///< Just for testing
//    static std::string year();
//    ------------------------------

    AppBook();
private:

    explicit AppBook(const std::string &book_id);


    static Book::Result SetupBookApiJournal();
    Section::Array Sections;
    AppBook::Section::Contents *current_stream{nullptr};
    config_data Conf;
    AppBook::ElementComponents Ec{0};
    static Book::Result Init();
    static AppBook *Application_Book;

};

namespace Book
{
[[maybe_unused]] AppBook::Section::Contents::Element &Error(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Out(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Warning(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Fatal(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Except(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Message(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Debug(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Info(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Comment(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Syntax(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Status(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Test(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Interrupted(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Aborted(std::source_location src = std::source_location::current());
[[maybe_unused]] AppBook::Section::Contents::Element &Segfault(std::source_location src = std::source_location::current());

AppBook& Select();

}