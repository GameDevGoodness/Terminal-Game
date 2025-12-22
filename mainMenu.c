#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    int textPerLine;
    int paddingX;
    int paddingY;
    int textOptionsCount;
    char *text;
    char *boxedText;

} textBox;

char* getNextWord(const char *text, int *index)
{    
    // get word size
    size_t textLen = strlen(text);
    int wordLen = 0;
    for (int i = *index; i < textLen && text[i] != '\n'; i++, wordLen++)
    {
        if (text[i] == ' ')
        {
            wordLen++;
            break;
        }
    }

    printf("word length: %i\n", wordLen);

    // there are no more words left on this line
    if (wordLen == 0)
    {
        (*index)++;
        return NULL;
    }

    // get word substring
    char *word = (char*)malloc(sizeof(char) * wordLen);
    strncpy(word, text + *index, wordLen);
    
    printf("word: %s\n", word);

    // update index
    *index += wordLen;
    
    if (wordLen + 1 < textLen && text[wordLen + 1] == '\n') (*index)++;

    return word;
}

void createMainMenu(textBox *menuTextBox)
{
    // size of the entire text box
    int boxSizeX = menuTextBox->paddingX * 2 + menuTextBox->textPerLine + 1;

    menuTextBox->boxedText = (char*)calloc(2048, sizeof(char));
    
    // top of box
    strcat(menuTextBox->boxedText, "╔");
    for (int x = 0; x < boxSizeX - 1; x++)
        strcat(menuTextBox->boxedText, "═");
    strcat(menuTextBox->boxedText, "╗\n");

    // y padding before main body
    for (int y = 0; y < menuTextBox->paddingY; y++)
    {
        strcat(menuTextBox->boxedText, "║");
        for (int x = 0; x < boxSizeX - 1; x++)
            strcat(menuTextBox->boxedText, " ");
        strcat(menuTextBox->boxedText, "║\n");
    }

    int textIndex = 0;
    size_t textLen = strlen(menuTextBox->text);

    // text options portion of box
    while (textIndex < textLen)
    {
        strcat(menuTextBox->boxedText, "║");

        // padding on left
        for (int x = 0; x < menuTextBox->paddingX; x++)
            strcat(menuTextBox->boxedText, " ");

        int currLineCount = 0;
        do
        {
            char *word = getNextWord(menuTextBox->text, &textIndex);

            if (word == NULL) break;

            int wordLen = strlen(word);
            if (currLineCount + wordLen < menuTextBox->textPerLine)
            {
                strcat(menuTextBox->boxedText, word);
                currLineCount += wordLen;
                free(word);
            }
            else
            {
                textIndex -= wordLen - 1;
                free(word);
                break;
            }
        } while (textIndex < textLen && menuTextBox->text[textIndex - 1] != '\n');
        
        // padding on right
        for (int x = 0; x < menuTextBox->paddingX + menuTextBox->textPerLine - currLineCount; x++)
            strcat(menuTextBox->boxedText, " ");

        strcat(menuTextBox->boxedText, "║\n");
    }

    // y padding after main body
    for (int y = 0; y < menuTextBox->paddingY; y++)
    {
        strcat(menuTextBox->boxedText, "║");
        for (int x = 0; x < boxSizeX - 1; x++)
            strcat(menuTextBox->boxedText, " ");
        strcat(menuTextBox->boxedText, "║\n");
    }

    // top of box
    strcat(menuTextBox->boxedText, "╚");
    for (int x = 0; x < boxSizeX - 1; x++)
        strcat(menuTextBox->boxedText, "═");
    strcat(menuTextBox->boxedText, "╝\n");

    menuTextBox->boxedText = (char*)realloc(menuTextBox->boxedText, strlen(menuTextBox->boxedText) * sizeof(char));

    printf("%s", menuTextBox->boxedText);
}