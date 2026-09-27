#pragma once

#include "raylib.h"
#include "CHIP_8.h"
#include "UILayout.h"
#include "fonts.h"

#include <string>

class Chip8App
{
public:
    Chip8App();
    ~Chip8App();



    int Run();

private:
    const int screenWidth = 950;
    const int screenHeight = 550;



    // =========================================================
    // Emulator
    // =========================================================

    CHIP_8 chip8;
    UILayout layout;

    // =========================================================
    // UI resources
    // =========================================================

    FontSet fonts;
    Texture2D logo{};

    // =========================================================
    // Application state
    // =========================================================

    bool shouldClose = false;

    std::string currentROM_Name = "empty";
    std::string currentROM_Path;

    // =========================================================
    // CPU timing
    // =========================================================

    float cpuHz = 700.0f;

    double cpuAccumulator = 0.0;
    double timerAccumulator = 0.0;

private:
    // =========================================================
    // Application
    // =========================================================

    void Initialize();
    void Shutdown();

    void Update();
    void Draw();

    // =========================================================
    // Emulator
    // =========================================================

    void UpdateKeyboard();
    void UpdateCPU(float deltaTime);
    void UpdateTimers(float deltaTime);

    // =========================================================
    // Drawing
    // =========================================================

    void DrawNavbar();
    void DrawControls();
    void DrawChip8Screen();
    void DrawStatusPanel();
    void DrawCPUInfo();
    void DrawRegisters();
    void DrawKeypad();
    void DrawFooter();

    // =========================================================
    // Actions
    // =========================================================

    void LoadROM();
    void Reset();
    void Step();

    // =========================================================
    // Utility
    // =========================================================

    std::string GetFileName(const std::string& path) const;
};