#include <string.h>

struct cell
{
    char* ch;
    int x, y;
    
    enum
    {
        empty,
        grass,
        wall,
        player
    } attribute;
};

void getCellType(struct cell* c)
{
    if (strcmp(c->ch, "▓") == 0)
    {
        c->attribute = player;
        printf("Success");
    }
}

struct player
{

};