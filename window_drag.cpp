#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "window_drag.h"

#include <commdlg.h>
#include <string>
#include <minwindef.h>

#pragma comment(lib, "Comdlg32.lib")

namespace
{
    HWND g_hwnd = nullptr;
    WNDPROC g_oldWndProc = nullptr;

    constexpr int NAVBAR_HEIGHT = 48;

    // Close button position from your UI:
    // x = screenWidth - 32 - 12
    // y = 8
    // width = 32
    // height = 32
    bool IsOverCloseButton(POINT point)
    {
        RECT clientRect{};
        GetClientRect(g_hwnd, &clientRect);

        int closeX = (clientRect.right - clientRect.left) - 32 - 12;
        int closeY = 8;
        int closeWidth = 32;
        int closeHeight = 32;

        return
            point.x >= closeX &&
            point.x <= closeX + closeWidth &&
            point.y >= closeY &&
            point.y <= closeY + closeHeight;
    }

    LRESULT CALLBACK WindowProc(
        HWND hwnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam)
    {
        switch (msg)
        {
        case WM_NCHITTEST:
        {
            LRESULT hit = CallWindowProc(
                g_oldWndProc,
                hwnd,
                msg,
                wParam,
                lParam
            );

            if (hit == HTCLIENT)
            {
                POINT point;

                // lParam contains screen coordinates.
                point.x = (int)(short)LOWORD(lParam);
                point.y = (int)(short)HIWORD(lParam);

                ScreenToClient(hwnd, &point);

                // Navbar area
                if (point.y >= 0 && point.y < NAVBAR_HEIGHT)
                {
                    // Don't turn the close button into a drag area.
                    if (IsOverCloseButton(point))
                    {
                        return HTCLIENT;
                    }

                    // Tell Windows this area behaves like a title bar.
                    return HTCAPTION;
                }
            }

            return hit;
        }
        }

        return CallWindowProc(
            g_oldWndProc,
            hwnd,
            msg,
            wParam,
            lParam
        );
    }
}

void EnableWindowDragging(void* windowHandle)
{
    if (windowHandle == nullptr)
        return;

    g_hwnd = static_cast<HWND>(windowHandle);

    g_oldWndProc = reinterpret_cast<WNDPROC>(
        SetWindowLongPtr(
            g_hwnd,
            GWLP_WNDPROC,
            reinterpret_cast<LONG_PTR>(WindowProc)
        )
        );
}

void DisableWindowDragging()
{
    if (g_hwnd != nullptr && g_oldWndProc != nullptr)
    {
        SetWindowLongPtr(
            g_hwnd,
            GWLP_WNDPROC,
            reinterpret_cast<LONG_PTR>(g_oldWndProc)
        );

        g_oldWndProc = nullptr;
        g_hwnd = nullptr;
    }

   
}

std::string OpenROMFileDialog()
{
    char fileName[MAX_PATH] = {};

    OPENFILENAMEA dialog = {};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = nullptr;
    dialog.lpstrFile = fileName;
    dialog.nMaxFile = MAX_PATH;

    dialog.lpstrFilter =
        "CHIP-8 ROMs (*.ch8;*.c8;*.rom)\0*.ch8;*.c8;*.rom\0"
        "All Files (*.*)\0*.*\0";

    dialog.nFilterIndex = 1;
    dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    dialog.lpstrInitialDir = "assets\\roms";

    if (GetOpenFileNameA(&dialog))
    {
        return std::string(fileName);
    }

    return {};
}