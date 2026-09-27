#pragma once
#include<string>
// Enables native Windows dragging from the top navbar.
// Pass the handle returned by raylib's GetWindowHandle().
void EnableWindowDragging(void* windowHandle);

// Restores the original Windows window procedure.
void DisableWindowDragging();


std::string OpenROMFileDialog();