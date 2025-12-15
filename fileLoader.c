#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.c"

board* loadLevelChunks(FILE*);

void buildLevel(char* filename)
{
    FILE* fp = fopen(filename, "r");
    
    int chunks = 0;

    board* boards = loadLevelChunks(fp);
    printBoard(&boards[0]);
    printBoard(&boards[1]);
    fclose(fp);
}

board* loadLevelChunks(FILE* fp)
{
    char **fileContent = NULL;
    board* boards = NULL;

    int boardCount = 0;
    char line[256];
    int rows = 0;
    bool readBoardSettings = false;

    while ((fgets(line, sizeof(line), fp)) != NULL)
    {
        // Allocate a new board if a board read symbol is found
        if (strchr(line, '&') != NULL)
        {
            rows = 0;
            readBoardSettings = false;
            boards = realloc(boards, sizeof(board) * (boardCount + 1));
        }

        if (readBoardSettings == false)
        {
            // Read board layout
            fileContent = realloc(fileContent, sizeof(char *) * (rows + 1));
            fileContent[rows] = (char*)malloc(sizeof(char) * strlen(line));
            strcpy(fileContent[rows], line);
            rows++;
        }
        else
        {
            // Read board settings
            if (strstr(line, "Link:"))
            {
                int x1, x2;
                int y1, y2;

                sscanf(line, "Link: %d %d %d %d", &x1, &y1, &x2, &y2);
                boards[boardCount - 1].cells[y1][x1].connector = true;
                boards[boardCount - 1].cells[y2][x2].connector = true;
            }
            else if (strcmp(line, "Spawn:"))
            {

            }
        }
        
        // Fill board with text info when board finish read symbol is found
        if (strchr(line, '~') != NULL)
        {
            populateBoard(fileContent, &boards[boardCount], rows);

            readBoardSettings = true;
            boardCount++;

            // release board memory
            for (int i = 0; i < rows; i++)
                free(fileContent[i]);
            free(fileContent);
            fileContent = NULL;
        }
    }

    return boards;
}