#if defined(_WIN32)
#define OS_WINDOWS 1
#elif defined(__linux__)
#define OS_LINUX 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include "fileLoader.c"
#include "mainMenu.c"

// Unicode/Terminal Helpers
#ifdef OS_WINDOWS
//#include <windows.h>
//#include <conio.h>

#elif OS_LINUX
#include <locale.h>
#include <ncurses.h>
#endif

void listenInputs(char*);

int main()
{
#if OS_WINDOWS
    SetConsoleOutputCP(CP_UTF8);
#elif OS_LINUX
    setlocale(LC_ALL, "en_US.utf8");
#endif

    buildLevel("Level0_1.txt");

    textBox *mainMenuTextBox = (textBox*)malloc(sizeof(textBox));
    mainMenuTextBox->text = "Why hello there!\nBrotato🔳";
    mainMenuTextBox->textPerLine = 20;
    mainMenuTextBox->paddingX = 4;
    mainMenuTextBox->paddingY = 1;
    mainMenuTextBox->textOptionsCount = 2;
   // createMainMenu(mainMenuTextBox);
    free(mainMenuTextBox);

    pthread_t inputThreadID;

    char playerInput;

    pthread_create(&inputThreadID, NULL, (void*)listenInputs, &playerInput);
    pthread_join(inputThreadID, NULL);

    return 0;
}

void listenInputs(char* input)
{
    while (true)
    {
        break;
        *input = getch();
        
        if (*input == 27) // Escape character
        {
            break;
        }
    }
    pthread_exit(NULL);
}