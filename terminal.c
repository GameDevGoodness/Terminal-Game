#include "include/terminal.h"

#if OS_WINDOWS
#include <conio.h>
#include <locale.h>
#include <windows.h>

#else
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

static struct termios originalTerminal;

#endif

#include <stdio.h>

void restoreTerminal()
{
#if !OS_WINDOWS
    // restore original Linux terminal settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTerminal);
#endif

    // restore the original cursor and return to the main terminal buffer
    printf("\033[?25h\033[?1049l");
    fflush(stdout);
}

void enableRawTerminalMode()
{
// Windows specific terminal settings
#if OS_WINDOWS
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    // setting the character locale
    setlocale(LC_ALL, ".UTF-8");

    // get the handle on the std output and enable virtual processing
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // disable line input and echo on Windows std input
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD inMode = 0;
    GetConsoleMode(hIn, &inMode);
    // flip the bits that allow echo and line buffering
    inMode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    SetConsoleMode(hIn, inMode);
#else
    // setting the character locale
    setlocale(LC_ALL, "");

    // get and save the current terminal attributes
    tcgetattr(STDIN_FILENO, &originalTerminal);
    struct termios raw = originalTerminal;

    // flip the bits that allow echo and line buffering
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
    // save current terminal cursor position; switch to and clear secondary
    // screen
    printf("\033[?1049h\033[?25l");
    fflush(stdout);
}

int getInput()
{
// get the character input from stdin
#if OS_WINDOWS
    return _getch();
#else
    unsigned char c;

    // read the first character in the file and return it
    if (read(STDIN_FILENO, &c, 1) == 1)
    {
        return c;
    }

    return -1;
#endif
}

bool isAnyKeyPressed()
{
// if on windows return if the keyboard was hit
#if OS_WINDOWS
    return _kbhit();
#else
    // set a zero timeout value to check for input immediately
    struct timeval tv = {0L, 0L};
    // set a file descriptor to get and set the stdin file
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
#endif
}