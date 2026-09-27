#include "Chip8App.h"

# if defined(WIN32) && !defined(_DEBUG)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#endif
int main()
{
    Chip8App app;
    return app.Run();
}