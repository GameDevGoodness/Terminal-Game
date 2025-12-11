#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** loadLevelChunk(char* fileName, int* row)
{
    FILE *fp = fopen(fileName, "r");
    
    char **fileContent = NULL;

    char line[256];
    *row = 0;

    while ((fgets(line, sizeof(line), fp)) != NULL)
    {
        fileContent = realloc(fileContent, sizeof(char*) * ((*row) + 1));
        fileContent[*row] = (char*)malloc(sizeof(char) * strlen(line));
        strcpy(fileContent[*row], line);

        (*row)++;
        if (strchr(line, 'E') != NULL)
            break;
    }

    fclose(fp);

    return fileContent;
}