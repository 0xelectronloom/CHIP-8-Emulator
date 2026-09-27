#pragma once

#include "raylib.h"

class FontSet
{
public:
    Font light{};
    Font reg{};
    Font med{};
    Font bold{};
    Font ebold{};

    void Load()
    {
        light = LoadFontEx(
            "assets/fonts/JetBrainsMono-Light.ttf",
            16,
            nullptr,
            0
        );

        reg = LoadFontEx(
            "assets/fonts/JetBrainsMono-Regular.ttf",
            14,
            nullptr,
            0
        );

        med = LoadFontEx(
            "assets/fonts/JetBrainsMono-Medium.ttf",
            14,
            nullptr,
            0
        );

        bold = LoadFontEx(
            "assets/fonts/JetBrainsMono-Bold.ttf",
            16,
            nullptr,
            0
        );

        ebold = LoadFontEx(
            "assets/fonts/JetBrainsMono-ExtraBold.ttf",
            16,
            nullptr,
            0
        );
    }

    void Unload()
    {
        UnloadFont(light);
        UnloadFont(reg);
        UnloadFont(med);
        UnloadFont(bold);
        UnloadFont(ebold);
    }
};