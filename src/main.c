#include "../include/terminal.h"

#if OS_WINDOWS
#include <windows.h>

#else
#include <unistd.h>

#endif

#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

typedef struct
{
    wchar_t glyph;
    bool doubleWide;
} Cell;

// Program-Wide Constants
#define SCREEN_WIDTH 137 // In Wide Chars
#define SCREEN_HEIGHT 32 // In Wide Chars

#define PLAY_AREA_WIDTH 110
#define PLAY_AREA_HEIGHT 23

static Cell Board[SCREEN_HEIGHT][SCREEN_WIDTH];

int main()
{
    // cleanup procedure when exiting the program
    atexit(restoreTerminal);
    // Lanch the game terminal
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
            printf("❤︎\n");
        }

        // perform game logic

// sleep for 0.5 seconds
#if OS_WINDOWS
        Sleep(16);
#else
        usleep(500000);
#endif
    }

    return 0;
}