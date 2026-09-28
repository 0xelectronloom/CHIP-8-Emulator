#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "Chip8App.h"

#include "window_drag.h"
#include "ui_styles.h"
#include <exception>
#include<format>

#define VERSION  "v0.1.0-beta.1"


Chip8App::Chip8App()
    : layout(screenWidth, screenHeight)
{
}

Chip8App::~Chip8App()
{
    Shutdown();
}
void Chip8App::Initialize()
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);

    InitWindow(
        screenWidth,
        screenHeight,
        "CHIP-8 Emulator"
    );

    SetTargetFPS(60);

    fonts.Load();

    EnableWindowDragging(GetWindowHandle());

    logo = LoadTexture("assets/icons/b.png");

    if (logo.id != 0)
    {
        SetTextureFilter(logo, TEXTURE_FILTER_POINT);
    }

    chip8.init();
    chip8.running = false;
}

void Chip8App::Shutdown()
{
    DisableWindowDragging();

    fonts.Unload();

    if (logo.id != 0)
    {
        UnloadTexture(logo);
    }

    CloseWindow();
}
int Chip8App::Run()
{
    Initialize();

    while (!WindowShouldClose() && !shouldClose)
    {
        Update();
        Draw();
    }

    return 0;
}
void Chip8App::Update()
{
    float deltaTime = GetFrameTime();

    UpdateKeyboard();
    UpdateCPU(deltaTime);
    UpdateTimers(deltaTime);
}

void Chip8App::UpdateKeyboard()
{
    chip8.key[0x1] = IsKeyDown(KEY_ONE);
    chip8.key[0x2] = IsKeyDown(KEY_TWO);
    chip8.key[0x3] = IsKeyDown(KEY_THREE);
    chip8.key[0xC] = IsKeyDown(KEY_FOUR);
    chip8.key[0x4] = IsKeyDown(KEY_Q);
    chip8.key[0x5] = IsKeyDown(KEY_W);
    chip8.key[0x6] = IsKeyDown(KEY_E);
    chip8.key[0xD] = IsKeyDown(KEY_R);
    chip8.key[0x7] = IsKeyDown(KEY_A);
    chip8.key[0x8] = IsKeyDown(KEY_S);
    chip8.key[0x9] = IsKeyDown(KEY_D);
    chip8.key[0xE] = IsKeyDown(KEY_F);
    chip8.key[0xA] = IsKeyDown(KEY_Z);
    chip8.key[0x0] = IsKeyDown(KEY_X);
    chip8.key[0xB] = IsKeyDown(KEY_C);
    chip8.key[0xF] = IsKeyDown(KEY_V);
}

void Chip8App::UpdateCPU(float deltaTime)
{
    if (!chip8.running)
        return;

    cpuAccumulator += deltaTime * cpuHz;

    while (cpuAccumulator >= 1.0)
    {
        chip8.emulateCycle();
        cpuAccumulator -= 1.0;
    }
}
void Chip8App::UpdateTimers(float deltaTime)
{
    timerAccumulator += deltaTime;

    constexpr double timerInterval = 1.0 / 60.0;

    while (timerAccumulator >= timerInterval)
    {
        chip8.updateTimers();
        timerAccumulator -= timerInterval;
    }
}

void Chip8App::Draw()
{
    BeginDrawing();

    ClearBackground(Color{ UI::Primary });

    DrawNavbar();
    DrawControls();
    DrawChip8Screen();
    DrawStatusPanel();
    DrawCPUInfo();
    DrawKeypad();
    DrawFooter();

    EndDrawing();
}

void Chip8App::DrawNavbar()
{
    // ---------------------------------------------------------
    // Navbar
    // ---------------------------------------------------------

    DrawRectangle(
        0,
        0,
        screenWidth - 1,
        40,
        Color{ UI::Secondary }
    );

    // Navbar bottom stroke
    DrawRectangle(
        0,
        39,
        screenWidth,
        1,
        Color{ UI::Stroke }
    );
     // ---------------------------------------------------------
     // Footer
     // ---------------------------------------------------------

    DrawRectangle(
        0,
        screenHeight - 35,
        screenWidth - 1,
        35,
        Color{ UI::Secondary }
    );


    // ---------------------------------------------------------
    // Outer border
    // ---------------------------------------------------------

    DrawRectangleLines(
        0,
        0,
        screenWidth,
        screenHeight,
        Color{ UI::Stroke }
    );

    // ---------------------------------------------------------
    // Navbar title
    // ---------------------------------------------------------

    DrawTextEx(
        fonts.ebold,
        "CHIP-8 EMULATOR",
        Vector2{ 20 + 10 + 5, (40 - 14) / 2 },
        16,
        1,
        Color{ UI::Accent }
    );
    DrawRectangle(20 + 10 + 5 + 130 - 5, (40 - 14) / 2
        , 35, 15, Color{ 41, 44, 41 ,255 });
    DrawRectangleLines
    (


        20 + 10 + 5 + 130 - 5, (40 - 14) / 2,

        35, 15
        ,

        Color{ UI::Stroke }
    );
    DrawTextEx(
        fonts.med,
        "BETA",
        Vector2{ 20 + 10 + 5 + 130, (40 - 10) / 2 },
        13,
        1,
        Color{ UI::SecondaryText }
    );
    DrawTextureEx(
        logo,
        Vector2{ 0, 0 },
        0.0f,
        40.0f / logo.width,
        WHITE
    );
    // ---------------------------------------------------------
    // Close button
    // ---------------------------------------------------------
    Rectangle closeButton = {
    screenWidth - 24.0f - 20 + 10,
    (40 - 24) / 2,
    24.0f,
    24.0f
    };

    int oldPadding = GuiGetStyle(BUTTON, TEXT_PADDING);
    UI::ApplyButtonStyle(UI::GhostButton);

    if (GuiButton(
        closeButton,
        GuiIconText(ICON_CROSS, "")
    ))
    {
        shouldClose = true;
    }

    GuiSetStyle(BUTTON, TEXT_PADDING, oldPadding);

    DrawCircle(closeButton.x - 80 - 50, (40) / 2 - 1, 4, (chip8.running) ? UI::Accent : Color{ 180, 55, 55, 255 });
    DrawTextEx(fonts.med, (chip8.running) ? "RUNNING" : "PAUSED", Vector2{ closeButton.x - 70 - 50, (40 - 12) / 2 }, 14, 1, (chip8.running) ? UI::Accent : Color{ 180, 55, 55, 255 });
    DrawTextEx(fonts.med, TextFormat("%d FPS", GetFPS()), Vector2{ closeButton.x - 60, (40 - 12) / 2 }, 14, 1, UI::SecondaryText);

}

void Chip8App::DrawControls()
{

    // ---------------------------------------------------------
    // Emulator controls
    // ---------------------------------------------------------
    UI::ApplyButtonStyle(UI::AccentButton);


    GuiSetFont(fonts.reg);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 14);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 0);

    if (GuiButton(
        Rectangle{ 32,  48 + 32 + 320 + 32, 60, 30 },
        "Run"
    ))

    {
        if (!currentROM_Path.empty()) {
            chip8.running = true;
        }
    }
    UI::ApplyButtonStyle(UI::DefaultButton);

    if (GuiButton(
        Rectangle{ 32 + 32 + 32 + 5 , 48 + 32 + 320 + 32, 60, 30 },
        "Pause"
    ))
    {
        // Pause emulator
        if (!currentROM_Path.empty()) {
            chip8.running = false;
        }
    }
    if (GuiButton(
        Rectangle{ 32 + 32 + 32 + 60 + 5 + 5 + 3, 48 + 32 + 320 + 32, 60, 30 },
        "Step"
    ))
    {
        Step();

    }
    if (GuiButton(
        Rectangle{ 32 + 32 + 32 + 5 * 2 + 60 * 2 + 11 , 48 + 32 + 320 + 32, 60, 30 },
        "Reset"

    ))
    {
        Reset();
    }
    GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, ColorToInt(UI::Secondary));
    GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, ColorToInt(UI::SecondaryText));
    GuiSetStyle(SLIDER, BASE_COLOR_FOCUSED, ColorToInt(RED));
    GuiSetStyle(SLIDER, BASE_COLOR_DISABLED, ColorToInt(UI::Accent));

    GuiSetStyle(SLIDER, BORDER_COLOR_FOCUSED, ColorToInt(UI::Accent));
    GuiSetStyle(SLIDER, BORDER_COLOR_PRESSED, ColorToInt(UI::Secondary));

    GuiSetStyle(SLIDER, SLIDER_WIDTH, 25);
    GuiSetStyle(SLIDER, TEXT_COLOR_FOCUSED, ColorToInt(UI::Accent));

    GuiSlider(Rectangle{ 32 + 32 + 32 + 5 * 2 + 60 * 4 + 15,48 + 32 + 320 + 32,275,30 }, "", TextFormat("%.0f Hz", cpuHz)
        , &cpuHz, 100.0f, 2000.f);
}
void Chip8App::DrawChip8Screen()
{
    // PANEL 
    Rectangle chip8Screen = {
        32,     // x
        48 + 32,     // y
        640,    // width
        320     // height
    };
    DrawRectangle(32, 48 + 32, 640, 320, Color{ 19, 22, 16 ,255 });

    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            if (chip8.gfx[y * 64 + x])
            {
                DrawRectangle(
                    chip8Screen.x + x * 10,
                    chip8Screen.y + y * 10,
                    10,
                    10,
                    UI::Accent
                );
            }
        }
    }
    // Retro effect
    for (int y = 0; y < chip8Screen.height; y += 2)
    {
        DrawRectangle(
            chip8Screen.x,
            chip8Screen.y + y,
            chip8Screen.width,
            1,
            Fade(UI::Stroke, 0.08f)
        );
    }

    //Border
    DrawRectangleRoundedLines
    (
        Rectangle{

        32,
        48 + 32,
        640 ,
        320 ,
        },
        0.02f, 50, 3,
        Color{ UI::Stroke }
        );

}

void Chip8App::DrawStatusPanel()
{

    // Status 
    Rectangle StatusPanel = {
        32 + 640 + 32,     // x
        40,     // y
        screenWidth - 32 - 640 - 32 - 1,    // width
        screenHeight - 48 - 1     // height
    };
    DrawRectangle(StatusPanel.x, StatusPanel.y, StatusPanel.width, StatusPanel.height, UI::Secondary);
    DrawRectangle(
        StatusPanel.x + 1,
        40,
        1,
        screenHeight - 48 - 32,
        Color{ UI::Stroke }
    );
    //ROM
    DrawTextEx(
        fonts.ebold,
        "ROM",
        Vector2{ 32 + 640 + 32 + (32 / 2), 40 + 32 / 2 },
        16,
        1,
        Color{ UI::Accent }
    );
    DrawRectangle(
        32 + 640 + 32 + (32 / 2),
        40 + 32 + 3,
        950 - 32 - 640 - 32 - 1 - (32 / 2) - 11,
        1,
        Color{ UI::Stroke }
    );

    DrawRectangle(StatusPanel.x + (32 / 2), 40 + 32 + 5 + 5 + 3
        , StatusPanel.width - (32 / 2) - 11, 35, Color{ 41, 44, 41 ,255 });
    DrawRectangleLines
    (


        StatusPanel.x + (32 / 2) + 1,
        40 + 32 + 5 + 5 + 3 + 1,

        StatusPanel.width - (32 / 2) - 11 - 1, 35 - 1
        ,

        Color{ UI::Stroke }
    );
    DrawTextEx(
        fonts.ebold,
        currentROM_Name.c_str(),
        Vector2{ 32 + 640 + 32 + (32 / 2) + 5 + 5, 40 + 32 + 5 + 5 + 3 + (35 / 2) - 7 },
        16,
        1,
        Color{ UI::Text }
    );
    UI::ApplyButtonStyle(UI::DefaultButton);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(Color{ 41, 44, 41 ,255 }));
    GuiSetFont(fonts.reg);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 14);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 0);

    if (GuiButton(
        Rectangle{ static_cast<float>(screenWidth) - (32 / 2) - 60 - 5 - 3,40 + 32 + 5 + 5 + 3 + (35 / 2) - (25 / 2) , 67, 25 },
        "Open File"
    ))
    {
        currentROM_Path = OpenROMFileDialog();

        if (!currentROM_Path.empty()) {

            size_t pos = currentROM_Path.find_last_of("\\/");

            //  std::cout << currentROM_Path.substr(pos + 1) <<std::endl;

            currentROM_Name = currentROM_Path.substr(pos + 1);
            chip8.init();
            chip8.load_rom(currentROM_Path);
        }



    }
   
    //Registers
    DrawTextEx(
        fonts.ebold,
        "Registers & Keypads",
        Vector2{ 32 + 640 + 32 + (32 / 2), 40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 20 + 20 * 4 + 18 },
        16,
        1,
        Color{ UI::Accent }
    );
    DrawRectangle(
        32 + 640 + 32 + (32 / 2),
        40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 20 + 20 * 4 + 18 + 20,
        950 - 32 - 640 - 32 - 1 - (32 / 2) - 11,
        1,
        Color{ UI::Stroke }
    );
    //

    int vi = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            DrawRectangle(StatusPanel.x + (32 / 2) + (StatusPanel.width - (32 / 2) - 11 - 20 * 8 + 3 - 5) * j
                , 40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 20 + 20 * 4 + 18 + 20 + 10 + 30 * i
                , StatusPanel.width - (32 / 2) - 11 - 20 * 8 - 5, 28, Color{ 41, 44, 41 ,255 });
            DrawRectangleLines
            (


                StatusPanel.x + (32 / 2) + 1 + (StatusPanel.width - (32 / 2) - 11 - 20 * 8 + 3 - 5) * j,
                40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 20 + 20 * 4 + 18 + 20 + 10 + 30 * i,

                StatusPanel.width - (32 / 2) - 11 - 20 * 8 - 1 - 5, 28 - 1
                ,

                Color{ UI::Stroke }
            );
            DrawTextEx(
                fonts.reg, TextFormat(
                    "V%d ",
                    vi
                ),
                Vector2{
                    StatusPanel.x + (32 / 2) + 8 + (StatusPanel.width - (32 / 2) - 11 - 20 * 8 + 3 - 5) * j ,
                    40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 20 + 20 * 4 + 18 + 20 + 10 + 8 + 30 * ((float)i)
                }, 14, 0,
                UI::Accent);

            //std::cout <<"---------------\n" << vi << " : " << hex_v <<"\n---------------" << std::endl;
            DrawTextEx(
                fonts.reg, TextFormat(
                    "%02X",
                    chip8.V[vi]
                ),
                Vector2{
                    StatusPanel.x + (32 / 2) + 8 + 25 + (StatusPanel.width - (32 / 2) - 11 - 20 * 8 + 3 - 5) * j ,
                    40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 20 + 20 * 4 + 18 + 20 + 10 + 8 + 30 * ((float)i)
                }, 14, 0,
                UI::SecondaryText);
            vi++;
        }
    }
}

void Chip8App::DrawCPUInfo()
{
    //CPU
    DrawTextEx(
        fonts.ebold,
        "CPU",
        Vector2{ 32 + 640 + 32 + (32 / 2), 40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 },
        16,
        1,
        Color{ UI::Accent }
    );
    DrawRectangle(
        32 + 640 + 32 + (32 / 2),
        40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20,
        950 - 32 - 640 - 32 - 1 - (32 / 2) - 11,
        1,
        Color{ UI::Stroke }
    );
    // PC
    DrawTextEx(fonts.bold, "PC", Vector2{ 32 + 640 + 32 + (32 / 2) + 2,40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 10 }, 16, 1, Color{ UI::SecondaryText });


    DrawTextEx(fonts.bold, TextFormat("0x%03X", chip8.pc), Vector2{ (static_cast<float>(screenWidth) - (32 / 2) - 32 - 5),40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 10.0f }, 16, 1, Color{ UI::SecondaryText });
    // I
    DrawTextEx(fonts.bold, "I", Vector2{ 32 + 640 + 32 + (32 / 2) + 2,40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 10 + 10 + 10 }, 16, 1, Color{ UI::SecondaryText });


    DrawTextEx(fonts.bold, TextFormat("0x%03X", chip8.I), Vector2{ (static_cast<float>(screenWidth) - (32 / 2) - 32 - 5),40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 10 + 10 + 10 }, 16, 1, Color{ UI::SecondaryText });
    // SP
    DrawTextEx(fonts.bold, "SP", Vector2{ 32 + 640 + 32 + (32 / 2) + 2,40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 10 + 10 + 10 + 20 }, 16, 1, Color{ UI::SecondaryText });


    DrawTextEx(fonts.bold, TextFormat("0x%03X", chip8.sp), Vector2{ (static_cast<float>(screenWidth) - (32 / 2) - 32 - 5),40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 10 + 10 + 10 + 20 }, 16, 1, Color{ UI::SecondaryText });
    // Delay
    DrawTextEx(fonts.bold, "Delay", Vector2{ 32 + 640 + 32 + (32 / 2) + 2,40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 10 + 10 + 10 + 20 * 2 }, 16, 1, Color{ UI::SecondaryText });


    DrawTextEx(fonts.bold, TextFormat("0x%03X", chip8.delay_timer), Vector2{ (static_cast<float>(screenWidth) - (32 / 2) - 32 - 5),40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 10 + 10 + 10 + 20 * 2 }, 16, 1, Color{ UI::SecondaryText });
    // Sound
    DrawTextEx(fonts.bold, "Sound", Vector2{ 32 + 640 + 32 + (32 / 2) + 2,40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 10 + 10 + 10 + 20 * 3 }, 16, 1, Color{ UI::SecondaryText });


    DrawTextEx(fonts.bold, TextFormat("0x%03X", chip8.sound_timer), Vector2{ (static_cast<float>(screenWidth) - (32 / 2) - 32 - 5),40 + 32 / 2 + 32 + 5 + 5 + 3 + 20 + 10 + 5 + 20 + 10 + 10 + 10 + 20 * 3 }, 16, 1, Color{ UI::SecondaryText });

}

void Chip8App::DrawKeypad()
{
    static const char* keypadNames[16] =
    {
        "1", "2", "3", "C",
        "4", "5", "6", "D",
        "7", "8", "9", "E",
        "A", "0", "B", "F"
    };

    constexpr int Columns = 4;
    constexpr int Rows = 4;

    float cellWidth =
        layout.keypadGrid.width / Columns;

    float cellHeight =
        layout.keypadGrid.height / Rows;

    for (int row = 0; row < Rows; ++row)
    {
        for (int column = 0;
            column < Columns;
            ++column)
        {
            int index =
                row * Columns + column;

            Rectangle cell = {
                layout.keypadGrid.x +
                    column * cellWidth,

                layout.keypadGrid.y +
                    row * cellHeight,

                cellWidth - 5,
                cellHeight - 5
            };

            bool pressed = chip8.key[index];

            Color background =
                pressed
                ? UI::Accent
                : Color{ 41, 44, 41, 255 };

            Color textColor =
                pressed
                ? UI::Primary
                : UI::SecondaryText;

            DrawRectangle(
                cell.x,
                cell.y,
                cell.width,
                cell.height,
                background
            );

            DrawRectangleLines(
                cell.x,
                cell.y,
                cell.width,
                cell.height,
                UI::Stroke
            );

            DrawTextEx(
                fonts.reg,
                keypadNames[index],
                Vector2{
                    cell.x + cell.width / 2.0f - 4,
                    cell.y + cell.height / 2.0f - 7+2
                },
                14,
                0,
                textColor
            );
        }
    }
}

void Chip8App::DrawFooter()
{

    // Footer bottom stroke
    Rectangle Footer = {
        0,
        screenHeight - 35,
        screenWidth - 1,
        35
    };
    DrawRectangle(
        0,
        screenHeight - 35,
        screenWidth,
        1,
        Color{ UI::Stroke }
    );
    DrawTextEx(
        fonts.med,
        VERSION,
        Vector2{ static_cast<float>(screenWidth) - 90 - 12  ,static_cast<float>(screenHeight) - (35 + 13) / 2 },
        14,
        1,
        Color{ UI::SecondaryText }
    );
    DrawTextEx(
        fonts.med,
        TextFormat(
            "PC: 0x%03X",
            chip8.pc
        ),
        //      (currentROM_Name.length() >= 14) ? (currentROM_Name.substr(0, 10) + "...").c_str() : currentROM_Name.c_str(),

        Vector2{ 15  ,static_cast<float>(screenHeight) - (35 + 13) / 2 },
        14,
        1,
        Color{ UI::SecondaryText }
    );
    DrawRectangle(
        15 + (47 / 7) * 10 + (47 / 7) * 2,//(currentROM_Name.length()%14)
        screenHeight - 35 + 25 / 2,
        1,
        Footer.height - 25,
        Color{ UI::Stroke }
    );
    DrawTextEx(
        fonts.med,
        TextFormat("CPU: %.0f Hz", cpuHz)
        ,
        //      (currentROM_Name.length() >= 14) ? (currentROM_Name.substr(0, 10) + "...").c_str() : currentROM_Name.c_str(),

        Vector2{ 15 + (47 / 7) * 10 + (47 / 7) * 4  ,static_cast<float>(screenHeight) - (35 + 13) / 2 },
        14,
        1,
        Color{ UI::SecondaryText }
    );
    DrawRectangle(
        15 + (47 / 7) * 10 + (47 / 7) * 15+20,//(currentROM_Name.length()%14)
        screenHeight - 35 + 25 / 2,
        1,
        Footer.height - 25,
        Color{ UI::Stroke }
    );
    DrawTextEx(
        fonts.med,
        ("AUDIO: OFF"),
        //      (currentROM_Name.length() >= 14) ? (currentROM_Name.substr(0, 10) + "...").c_str() : currentROM_Name.c_str(),

        Vector2{ 15 + (47 / 7) * 10 + (47 / 7) * 17+15+5 ,static_cast<float>(screenHeight) - (35 + 13) / 2 },
        14,
        1,
        Color{ 180, 55, 55, 255 }
    );
}

void Chip8App::LoadROM()
{
    std::string path = OpenROMFileDialog();

    if (path.empty())
        return;

    try
    {
        chip8.init();
        chip8.load_rom(path);

        currentROM_Path = path;
        currentROM_Name = GetFileName(path);

        chip8.running = false;

        cpuAccumulator = 0.0;
        timerAccumulator = 0.0;
    }
    catch (const std::exception& e)
    {
        TraceLog(
            LOG_ERROR,
            "Failed to load ROM: %s",
            e.what()
        );
    }
}

void Chip8App::Reset()
{
    if (currentROM_Path.empty())
        return;

    chip8.running = false;

    chip8.init();
    chip8.load_rom(currentROM_Path);

    cpuAccumulator = 0.0;
    timerAccumulator = 0.0;
}
void Chip8App::Step()
{
    chip8.emulateCycle();
}

std::string Chip8App::GetFileName(
    const std::string& path
) const
{
    size_t position =
        path.find_last_of("\\/");

    if (position == std::string::npos)
        return path;

    return path.substr(position + 1);
}
