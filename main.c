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

void listenInputs(char*);

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "en_US.utf8");

    char* mainHub = loadFileContents("MainHub.txt");

    printf("%s", mainHub);

    pthread_t inputThreadID;

    char playerInput;

    pthread_create(&inputThreadID, NULL, (void*)listenInputs, &playerInput);
    pthread_join(inputThreadID, NULL);

    free(mainHub);
    
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