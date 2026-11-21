#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "board.c"

board* loadLevelChunks(FILE*, int*);
void connectChunks(board*, int);

void buildLevel(char* filename)
{
    FILE* fp = fopen(filename, "r");
    
    int chunks;
    board* boards = loadLevelChunks(fp, &chunks);

    connectChunks(boards, chunks);

    fclose(fp);
}

board* loadLevelChunks(FILE* fp, int* boardCount)
{
    char **fileContent = NULL;
    board* boards = NULL;

    *boardCount = 0;
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
            boards = realloc(boards, sizeof(board) * (*boardCount + 1));
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
                boards[*boardCount - 1].xCon = realloc(boards[*boardCount - 1].xCon, 2);
                boards[*boardCount - 1].yCon = realloc(boards[*boardCount - 1].yCon, 2);
            }
            else if (strcmp(line, "Spawn:"))
            {

            }
        }
        
        // Fill board with text info when board finish read symbol is found
        if (strchr(line, '~') != NULL)
        {
            populateBoard(fileContent, &boards[*boardCount], rows);

            readBoardSettings = true;
            (*boardCount)++;

            // release board memory
            for (int i = 0; i < rows; i++)
                free(fileContent[i]);
            free(fileContent);
            fileContent = NULL;
        }
    }

    return boards;
}

void connectChunks(board* boards, int chunks)
{
    for (int i = 0; i < chunks; i++)
    {
        board chunk = boards[i];

        printBoard(&boards[i]);
        // for (int j = 0; j < chunk.rows; j++)
        // {
        //     for (int k = 0; k < chunk.cols[j]; k++)
        //     {
        //     }
        // }
    }
}