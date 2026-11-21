#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef unsigned char byte;

typedef struct
{
    struct cell** cells;
    int rows;
    int* cols;

    // To hold which cells are connectors
    int* xCon;
    int* yCon;
} board;

struct cell
{
    char base[5];
    char overlap[5];
    void *attributeData;
    int x, y;
    
    enum
    {
        empty,
        grass,
        wall,
        player
    } attribute;
};

struct vitalityStats
{
    // health data
    byte maxHealth;
    byte currHealth;
    float healPerTurn;
    float currHeal;

    // armor data
    byte maxArmor;
    byte currArmor;

    // stamina data
    byte maxStamina;
    byte currStamina;
    float staminaRegenPerTurn;
    float currStaminaRegen;
};

struct player
{
    struct vitalityStats vitals;
};

void getCellType(struct cell cell)
{
    if (strcmp(cell.base, "▓") == 0)
    {
        cell.attribute = player;
    }
}

void populateBoard(char** level, board* board, int rows)
{
    // Assign and allocate the rows of the board
    board->rows = rows;
    board->cells = (struct cell **)malloc(sizeof(struct cell *) * rows);
    board->cols = (int *)malloc(rows * sizeof(int));
    board->xCon = NULL;
    board->yCon = NULL;
    
    for (int i = 0; i < rows; i++)
    {
        int* skips = (int*)malloc(strlen(level[i]) * sizeof(int));
        int skipLen = 0;
        
        for (int j = 0; j < strlen(level[i]); j += skips[skipLen], skipLen++)
        {
            byte first_byte[1];
            
            memcpy(first_byte, level[i] + j, 1);
            
            int skipFactor = 0;
            // one byte ascii
            if ((first_byte[0] & 0x80) == 0x00)
                skipFactor = 1;
            // 2-byte character
            else if ((first_byte[0] & 0xE0) == 0xC0)
                skipFactor = 2;
            // 3-byte character
            else if ((first_byte[0] & 0xF0) == 0xE0)
                skipFactor = 3;
            // 4-byte character
            else
                skipFactor = 4;
            
            skips[skipLen] = skipFactor;
        }

        skips = realloc(skips, skipLen * sizeof(int));
        
        // Assign and allocate the board columns of each row
        board->cols[i] = skipLen;
        board->cells[i] = (struct cell *)malloc(sizeof(struct cell) * skipLen);
        
        int bytePos = 0;
        for (int j = 0; j < board->cols[i]; j++)
        {
            memset(board->cells[i][j].base, 0, 5);
            memset(board->cells[i][j].overlap, 0, 5);
            memcpy(board->cells[i][j].base, level[i] + bytePos, skips[j]);
            board->cells[i][j].x = j + 1;
            board->cells[i][j].y = i + 1;

            getCellType(board->cells[i][j]);
            bytePos += skips[j];
        }

        free(skips);
    }
}

void insertCellOverlap(board* board, int x, int y, char* overlap)
{
    memset(board->cells[y][x].overlap, 0, 5);
    strcpy(board->cells[y][x].overlap, overlap);
}

void deleteCellOverlap(board* board, int x, int y)
{
    memset(board->cells[y][x].overlap, 0, 5);
}

void printBoard(board* board)
{
    for (int i = 0; i < board->rows; i++)
    {
        for (int j = 0; j < board->cols[i]; j++)
        {
            // if (strcmp(board->cells[i][j].base, "\n") == 0)
            //     printf("\n");
            // else
            //     printf("x%dy%d ", board->cells[i][j].x, board->cells[i][j].y);

            if (board->cells[i][j].overlap[0] == '\0')
                printf("%s", board->cells[i][j].base);
            else
                printf("%s", board->cells[i][j].overlap);
        }
    }
}

void freeBoard(board *board)
{
    for (int i = 0;  i < board->rows; i++)
    {
        free(board->cells[i]);
    }
    
    free(board->cols);
    free(board->cells);
}