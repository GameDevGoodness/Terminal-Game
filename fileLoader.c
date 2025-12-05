#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* loadFileContents(char* fileName)
{
    FILE *fp = fopen(fileName, "r");
    
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char* fileContent = (char*)malloc(fileSize + 1);
    
    fread(fileContent, 1, fileSize, fp);

    fileContent[fileSize] = '\0';
    
    fclose(fp);

    return fileContent;
}