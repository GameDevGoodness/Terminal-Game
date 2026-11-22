#include "include/terminal.h"

#if OS_WINDOWS
#include <windows.h>

#else
#include <unistd.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

typedef struct
{
    wchar_t glyph;
    bool doubleWide;
} Cell;

// Program-Wide Constants
#define SCREEN_WIDTH 120 // In Wide Chars
#define SCREEN_HEIGHT 20 // In Wide Chars

static Cell Board[SCREEN_HEIGHT][SCREEN_WIDTH];

int main()
{
    // cleanup procedure when exiting the program
    atexit(restoreTerminal);
    enableRawTerminalMode();

    char input;

    while (true)
    {
        if (isAnyKeyPressed())
        {
            input = getInput();

            if (input == 27)
                break;

            // input logic
            printf("test\n");
        }

        // perform game logic

// sleep for 16 ms (60 fps)
#if OS_WINDOWS
        Sleep(16);
#else
        usleep(16000);
#endif
    }

    return 0;
}