//
// Created by oldlonecoder on 24-04-08.
//

//#ifndef APPBOOK_CWINDOW_H
//#define APPBOOK_CWINDOW_H

/******************************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                                        *
 *   serge.lussier@oldlonecoder.club                                                      *
 *                                                                                        *
 *                                                                                        *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge Lussier)                                                        *
 *   and no one else then not even {copilot, chatgpt, or any other AI calamities}         *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply To the Code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/

#pragma once
#include <AppBook/Util/Geometry.h>
#include <AppBook/Util/Object.h>
#include <AppBook/Utf/Cadres.h>
#include <AppBook/Utf/Glyphes.h>
namespace Book::ConIO
{


class APPBOOK_EXPORTS CWindow: public Util::Object
{
    Rect    R{};
    using Type = uint32_t*;



public:


    CWindow() = default;
    CWindow(Util::Object* ParentObj, const std::string& Id);
    ~CWindow() override;

    Book::Result Alloc();
    Book::Result ReAlloc();

    void Clear();


    void SetGeometry(Dim Wh);



    struct APPBOOK_EXPORTS Char
    {
        using Type = uint32_t;
        Type  Mem{0x040120};


        static constexpr uint32_t CharMask	= 0x000000FF;
        static constexpr uint32_t FGMask	= 0x0000FF00;
        static constexpr uint32_t BGMask	= 0x00FF0000;
        static constexpr uint32_t CMask		= 0x00FFFF00;
        static constexpr uint32_t AttrMask	= 0xFF000000;
        static constexpr uint32_t UTFMASK	= 0x11000000;
        // ==================Attributes==========================
        static constexpr uint32_t UGlyph 	= 0x01000000;
        static constexpr uint32_t Underline = 0x02000000;
        static constexpr uint32_t Stroke 	= 0x04000000;
        static constexpr uint32_t Blink 	= 0x08000000;
        static constexpr uint32_t Accent 	= 0x10000000;
        static constexpr uint32_t Frame 	= 0x20000000;
        // ==============Bit Shift ============================
        static constexpr int FGShift = 0x08;
        static constexpr int BGShift = 0x10;
        static constexpr int ATShift = 0x18;

        Char() =default;

        explicit Char(CWindow::Char::Type _Ch);
        explicit Char(CWindow::Char* _C);
        ~Char() =default;

        CWindow::Char& SetFg(Color::Code fg_);
        CWindow::Char& SetBg(Color::Code bg_);

        [[maybe_unused]] CWindow::Char& SetAttributes(CWindow::Char::Type d_);

        [[maybe_unused]] CWindow::Char& SetColors(Color::Pair&& c_);

        [[maybe_unused]] CWindow::Char& SetColors(Color::Pair& c_);

        CWindow::Char& operator=(CWindow::Char::Type d_);
        CWindow::Char& operator=(CWindow::Type d_);
        CWindow::Char& operator=(char d_);

        [[nodiscard]] Color::Code Fg() const;
        [[nodiscard]] Color::Code Bg() const;

        [[maybe_unused]] [[nodiscard]] Color::Pair Colors() const;

        [[maybe_unused]] [[nodiscard]] Utf::Glyph::Type IconID() const;

        [[maybe_unused]] [[nodiscard]] Utf::AccentFR::Type AccentID() const;
        [[nodiscard]] uint8_t Ascii() const;

        CWindow::Char::Type& operator*() { return Mem; }

        [[maybe_unused]] [[nodiscard]] uint16_t Attributes() const;

        CWindow::Char& operator << (Utf::Glyph::Type i_);
        CWindow::Char& operator << (Utf::AccentFR::Type a_);
        CWindow::Char& operator << (char c) { Mem = (Mem & ~CharMask) | c; return *this; }

        [[maybe_unused]] [[nodiscard]] std::string RenderColors() const;

        [[maybe_unused]] CWindow::Char& ResetAttributes(CWindow::Char::Type bits_);
        explicit operator std::string() const;
        [[nodiscard]] std::string Details() const;
        using Matrix = std::vector<std::vector<CWindow::Char>>;

    };

    int Width();
    int Height();

    struct APPBOOK_EXPORTS Pencil : public Util::Object
    {
        CWindow* Window{nullptr};
        Rect     R{};
        Point    CursorXY{}; ///< The Origin is {0,0}
        [[nodiscard]] int Width() const;
        [[nodiscard]] int Height() const;
        CWindow::Char::Type A{0x010420};


        Pencil(CWindow* W, CWindow::Char::Type DefaultAttr, Rect Sub);
        ~Pencil() override = default;

        Pencil& operator << (const std::string& Input);
        Pencil& operator << (CWindow::Char C);
        Pencil& operator << (char C);
        Pencil& operator << (Color::Code C);
        Pencil& operator << (Color::Pair Colors);
        Pencil& operator << (Utf::Glyph::Type Ic);
        Pencil& operator << (Utf::AccentFR::Type Ac);
        Pencil& operator << (Utf::Cadre::Index If);

        Point Position(Point XY={});
        void Clear(CWindow::Char::Type A = 0x010420);

        bool operator++();
        bool operator++(int);
        bool operator--();
        bool operator--(int);

    };

    std::vector<CWindow::Char>& operator[](size_t Line);

    CWindow::Pencil& BeginWrite(Rect Geom={}, CWindow::Char::Type Attr=0x010420);
    static Book::Result     EndWrite(CWindow::Pencil& Pen);

    Book::Result operator >>(StrAcc& Acc);
private:
    CWindow::Char::Matrix Buffer{};
    CWindow::Char::Type   A{0x010220};



};

} // Book::ConIO


//#endif //APPBOOK_CWINDOW_H
