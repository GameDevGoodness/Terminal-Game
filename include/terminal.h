#ifndef TERMINAL_H
#define TERMINAL_H

#if defined(_WIN32)
#define OS_WINDOWS 1
#else
#define OS_WINDOWS 0
#endif

void restoreTerminal();
void enableRawTerminalMode();
int getInput();
bool isAnyKeyPressed();

#endif