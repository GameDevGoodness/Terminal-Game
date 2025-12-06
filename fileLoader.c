#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* loadLevelChunk(char* fileName)
{
    FILE *fp = fopen(fileName, "r");
    
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char* fileContent = (char*)malloc(fileSize);

    int chCounter;
    int deadSpaceCnt = 0;
    for (chCounter = 0; chCounter < fileSize; chCounter++)
    {
        char ch = fgetc(fp);

        if (ch == 'E')
            break;
        
        if (ch != ' ')
            fileContent[chCounter-deadSpaceCnt] = ch;
        else
            deadSpaceCnt++;
    }
    
    fclose(fp);
    
    chCounter -= deadSpaceCnt;

    fileContent[chCounter + 1] = '\0';
    fileContent = realloc(fileContent, chCounter + 1);

    return fileContent;
}