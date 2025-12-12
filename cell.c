#include <string.h>

typedef unsigned char byte;

struct cell
{
    char* ch;
    void* attributeData;
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