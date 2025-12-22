#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <conio.h>

#elif defined(__linux__)
#include <ncurses.h>

#endif

#include <pthread.h>

// Unicode/Terminal Helpers
#include <locale.h>

#include "fileLoader.c"
#include "mainMenu.c"

void listenInputs(char*);

int main()
{
#if defined(_WIN32) || defined(_WIN64)
    SetConsoleOutputCP(CP_UTF8);
#elif defined(__linux__)
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