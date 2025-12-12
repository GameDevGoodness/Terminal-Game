#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generateWorld()
{
    FILE *fp = fopen("_World.txt", "w");
    
    char *fileContent = malloc(1024 * sizeof(char));

    for (int i = 0; i < sizeof(fileContent); i++)
    {
        fileContent[i] = ".";
    }

    fclose(fp);
}