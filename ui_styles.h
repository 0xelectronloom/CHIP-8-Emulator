#pragma once


namespace UI
{
    // ---------------------------------------------------------
    // Button Style
    // ---------------------------------------------------------

    struct ButtonStyle
    {
        // Background colors
        Color normal;
        Color focused;
        Color pressed;

        // Border colors
        Color borderNormal;
        Color borderFocused;
        Color borderPressed;

        // Text
        Color text;

        // Layout
        int borderWidth = 1;
        int textPadding = 4;
    };


    // ---------------------------------------------------------
    // Apply button style to raygui
    // ---------------------------------------------------------

    inline void ApplyButtonStyle(const ButtonStyle& style)
    {
        // Background
        GuiSetStyle(
            BUTTON,
            BASE_COLOR_NORMAL,
            ColorToInt(style.normal)
        );

        GuiSetStyle(
            BUTTON,
            BASE_COLOR_FOCUSED,
            ColorToInt(style.focused)
        );

        GuiSetStyle(
            BUTTON,
            BASE_COLOR_PRESSED,
            ColorToInt(style.pressed)
        );


        // Border
        GuiSetStyle(
            BUTTON,
            BORDER_COLOR_NORMAL,
            ColorToInt(style.borderNormal)
        );

        GuiSetStyle(
            BUTTON,
            BORDER_COLOR_FOCUSED,
            ColorToInt(style.borderFocused)
        );

        GuiSetStyle(
            BUTTON,
            BORDER_COLOR_PRESSED,
            ColorToInt(style.borderPressed)
        );


        // Border width
        GuiSetStyle(
            BUTTON,
            BORDER_WIDTH,
            style.borderWidth
        );


        // Text
        GuiSetStyle(
            BUTTON,
            TEXT_COLOR_NORMAL,
            ColorToInt(style.text)
        );


        // Text padding
        GuiSetStyle(
            BUTTON,
            TEXT_PADDING,
            style.textPadding
        );
    }


    // ---------------------------------------------------------
    // Colors
    // ---------------------------------------------------------

    constexpr Color Primary =
    {
        24, 26, 24, 255
    };

    constexpr Color Secondary =
    {
        32, 35, 32, 255
    };

    constexpr Color Stroke =
    {
        65, 70, 65, 255
    };

    constexpr Color Accent =
    {
        166, 195, 111, 255
    };

    constexpr Color Text =
    {
        214, 217, 212, 255
    };

    constexpr Color SecondaryText =
    {
        133, 140, 132, 255
    };


    // ---------------------------------------------------------
    // Default Button
    // ---------------------------------------------------------

    constexpr ButtonStyle DefaultButton =
    {
        // Background
        Secondary,
        { 42, 46, 42, 255 },
        { 50, 54, 50, 255 },

        // Border
        Stroke,
        { 90, 96, 90, 255 },
        { 110, 116, 110, 255 },

        // Text
        Text,

        // Border width
        1,

        // Text padding
        4
    };


    // ---------------------------------------------------------
    // Accent Button
    // ---------------------------------------------------------

    constexpr ButtonStyle AccentButton =
    {
        // Background
        Accent,
        { 180, 210, 125, 255 },
        { 140, 170, 90, 255 },

        // Border
        Accent,
        { 190, 220, 135, 255 },
        { 140, 170, 90, 255 },

        // Text
        Primary,

        // Border width
        1,

        // Text padding
        4
    };


    // ---------------------------------------------------------
    // Danger Button
    // ---------------------------------------------------------

    constexpr ButtonStyle DangerButton =
    {
        // Background
        { 180, 55, 55, 255 },
        { 200, 65, 65, 255 },
        { 150, 40, 40, 255 },

        // Border
        { 120, 40, 40, 255 },
        { 220, 80, 80, 255 },
        { 110, 30, 30, 255 },

        // Text
        WHITE,

        // Border width
        1,

        // Text padding
        2
    };


    // ---------------------------------------------------------
    // Ghost Button
    // ---------------------------------------------------------

    constexpr ButtonStyle GhostButton =
    {
        // Background
        { 0, 0, 0, 0 },
        Secondary,
        { 42, 46, 42, 255 },

        // Border
        Stroke,
        Accent,
        Accent,

        // Text
        Text,

        // Border width
        1,

        // Text padding
        4
    };
}