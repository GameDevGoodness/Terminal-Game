#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <pthread.h>

// Unicode/Terminal Helpers
#include <locale.h>
#include <wchar.h>
#include <windows.h>

#include "fileLoader.c"
#include "mainMenu.c"

void listenInputs(char*);

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "en_US.utf8");
    
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
        *input = getch();
        
        if (*input == 27) // Escape character
        {
            break;
        }
    }
    pthread_exit(NULL);
}