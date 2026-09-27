#pragma once

#include "raylib.h"

class UILayout
{
public:






    // Registers / keypad
    Rectangle registersHeader;
    Rectangle registersGrid;
    Rectangle keypadGrid;

    UILayout(int windowWidth, int windowHeight);
};