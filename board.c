#include <string.h>
#include <stdlib.h>

typedef unsigned char byte;

struct board
{
    struct cell** cells;
    int rows;
    int* cols;
};

struct cell
{
    char ch[5];
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
    if (strcmp(cell.ch, "▓") == 0)
    {
        cell.attribute = player;
    }
}

void populateBoard(char** level, struct board* board, int rows)
{
    // Assign and allocate the rows of the board
    board->rows = rows;
    board->cells = (struct cell **)malloc(sizeof(struct cell *) * rows);
    board->cols = (int *)malloc(rows * sizeof(int));

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
            memset(board->cells[i][j].ch, 0, 5);
            memcpy(board->cells[i][j].ch, level[i] + bytePos, skips[j]);
            board->cells[i][j].x = j + 1;
            board->cells[i][j].y = i + 1;

            getCellType(board->cells[i][j]);
            bytePos += skips[j];
        }

        free(skips);
    }
}

void printBoard(struct board* board)
{
    for (int i = 0; i < board->rows; i++)
    {
        for (int j = 0; j < board->cols[i]; j++)
        {
            printf("%s", board->cells[i][j].ch);
        }
    }
}

void freeBoard(struct board *board)
{
    for (int i = 0;  i < board->rows; i++)
    {
        free(board->cells[i]);
    }
    
    free(board->cols);
    free(board->cells);
}