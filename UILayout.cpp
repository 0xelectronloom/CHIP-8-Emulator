#include "UILayout.h"

UILayout::UILayout(int windowWidth, int windowHeight)
{
    // =========================================================
    // Global layout constants
    // =========================================================

    constexpr float margin = 32.0f;
    constexpr float gap = 32.0f;

    constexpr float navbarHeight = 40.0f;
    constexpr float footerHeight = 35.0f;

    constexpr float chip8Width = 640.0f;
    constexpr float chip8Height = 320.0f;

    constexpr float panelPadding = 16.0f;



 
    // =========================================================
    // Registers section
    // =========================================================

    // =========================================================
    // Keypad section
    // =========================================================

    keypadGrid = {
         32 + 640 + 32 + (32 / 2),
         40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 20 + 20 * 4 + 18 + 20 + 10 + 30 * 5 + 20-30-13 ,
       226.5f,
        65.0f+15
    };
}