#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <windows.h>

#define SCREEN_WIDTH 130
#define SCREEN_HEIGHT 10

typedef struct
{
    wchar_t glyph;
    int is_hidden;
} CharCell;

CharCell screen[SCREEN_HEIGHT][SCREEN_WIDTH];

// Helper to check if a wchar_t is the start of a Windows UTF-16 surrogate pair
int is_high_surrogate(wchar_t ch)
{
    return (ch >= 0xD800 && ch <= 0xDBFF);
}

// Calculates the true visual cell width of a character on screen (1 or 2)
int get_visual_width(wchar_t high, wchar_t low)
{
    if (high < 32)
        return 0;

    // If it's a standard surrogate pair, calculate its absolute Unicode Code Point
    if (is_high_surrogate(high) && low != 0)
    {
        unsigned int codepoint = (((high - 0xD800) << 10) | (low - 0xDC00)) + 0x10000;

        // True Emojis live primarily in the U+1F000 to U+1FAFF range
        if (codepoint >= 0x1F000 && codepoint <= 0x1FAFF)
        {
            return 2; // Emojis like 🏪 and 🔳 take 2 cells
        }
        return 1; // Historic symbols like 𖧧 and 𖠰 take up only 1 cell visually!
    }

    // Explicit check for single-wchar_t symbols that are visually double-width (East Asian / CJK / Fullwidth blocks)
    // 0xFE30 to 0xFE4F covers full-width vertical/wavy punctuation lines like '﹏'
    if (high >= 0xFE30 && high <= 0xFE4F)
        return 2;
    if (high >= 0x1100 && high <= 0x115F) // Hangul Jamo
        return 2;
    if (high >= 0x2300 && high <= 0x23FF) // Miscellaneous Technical symbols (Some render wide)
        return 1;                         // Explicitly enforce single width for game borders if needed
    if (high >= 0x2600 && high <= 0x27BF)
        return 2;                         // Standard miscellaneous emojis
    if (high >= 0x3000 && high <= 0x9FFF) // CJK Symbols, Punctuation, Ideographs
        return 2;
    if (high >= 0xFF00 && high <= 0xFFEF) // Fullwidth ASCII variants
        return 2;

    return 1; // Normal ASCII and standard text characters
}

void clear_buffer()
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            screen[y][x].glyph = L' ';
            screen[y][x].is_hidden = 0;
        }
    }
}

void write_string(int x, int y, const wchar_t *str)
{
    int current_x = x;
    while (*str != L'\0' && current_x < SCREEN_WIDTH)
    {
        wchar_t ch = *str;

        screen[y][current_x].glyph = ch;
        screen[y][current_x].is_hidden = 0;

        // If this is a surrogate pair, store the second half in the next hidden slot
        if (is_high_surrogate(ch) && *(str + 1) != L'\0' && (current_x + 1) < SCREEN_WIDTH)
        {
            str++;
            screen[y][current_x + 1].glyph = *str;
            screen[y][current_x + 1].is_hidden = 1;
            current_x++;
        }

        current_x++;
        str++;
    }
}

void draw_screen()
{
    // Allocate a single buffer large enough for the whole frame + ANSI codes
    // 130 width * 10 height * ~8 bytes max per cell = ~10.4 KB. 32 KB is plenty.
    char frame_buffer[32768];
    int offset = 0;

    // 1. Hide cursor (\033[?25l) and move to top-left (\033[H)
    offset += snprintf(frame_buffer + offset, sizeof(frame_buffer) - offset, "\033[?25l\033[H");

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            if (screen[y][x].is_hidden)
            {
                continue;
            }

            wchar_t wch_buf[2] = {0};
            int char_count = 1;
            wch_buf[0] = screen[y][x].glyph;

            if (x + 1 < SCREEN_WIDTH && screen[y][x + 1].is_hidden)
            {
                wch_buf[1] = screen[y][x + 1].glyph;
                char_count = 2;
            }

            // Convert directly into the frame_buffer instead of a temp buffer
            int bytes = WideCharToMultiByte(
                CP_UTF8, 0, wch_buf, char_count,
                frame_buffer + offset, sizeof(frame_buffer) - offset - 16,
                NULL, NULL);

            if (bytes > 0)
            {
                offset += bytes;
                int visual_w = get_visual_width(wch_buf[0], wch_buf[1]);
                wchar_t current_glyph = screen[y][x].glyph;

                // Append padding directly to the frame_buffer
                if (current_glyph == L'═' || current_glyph == L'╔' || current_glyph == L'╚' ||
                    current_glyph == L'╦' || current_glyph == L'╩' || current_glyph == L'╬')
                {
                    // UTF-8 encoding for '═' (U+2550) is \xE2\x95\x90
                    memcpy(frame_buffer + offset, "\xE2\x95\x90", 3);
                    offset += 3;
                }
                else if (visual_w != 2)
                {
                    frame_buffer[offset++] = ' ';
                }
            }
        }
        frame_buffer[offset++] = '\n';
    }

    // 2. Write the entire frame to stdout in ONE single system call
    fwrite(frame_buffer, 1, offset, stdout);
    fflush(stdout);
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // 1. Switch to the Alternate Screen Buffer (disables scrollback history)
    //    and hide the cursor (\033[?25l)
    printf("\033[?1049h\033[?25l");
    fflush(stdout);

    bool odd = false;

    while (true)
    {
        clear_buffer();

        write_string(0, 0, L"╔═════════════════╦═══════════════════╗");
        write_string(0, 1, L"║🏪𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧║");
        write_string(0, 2, L"║𖧧𖧧𖧧𖧧𖠰𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧🔳𖧧𖧧║");
        write_string(0, 3, L"║𖧧𖧧𖧧𖧧𖠰𖠰𖧧𖧧𖧧𖧧𖧧﹏﹏𖧧𖧧𖧧𖧧║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖠰𖠰𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧║");
        write_string(0, 4, L"║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧﹏﹏﹏﹏𖧧𖧧𖧧𖧧║𖧧𖠰𖧧𖧧𖧧𖠰𖠰𖠰𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧║");
        write_string(0, 5, L"║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧║");
        write_string(0, 6, L"║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖠰𖧧𖧧║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖠰𖧧𖧧𖧧𖧧𖧧𖧧𖧧║");
        write_string(0, 7, L"║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧║");

        if (odd)
        {
            write_string(0, 8, L"║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧║");
            odd = false;
        }
        else
        {
            write_string(0, 8, L"║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧😁𖧧𖧧𖧧𖧧𖧧𖧧𖧧║𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧𖧧║");
            odd = true;
        }

        write_string(0, 9, L"╚═════════════════════════════════════╝");

        draw_screen();
        Sleep(100);
    }

    // 2. Restore the cursor (\033[?25h) and return to the main terminal buffer (\033[?1049l)
    printf("\033[?25h\033[?1049l");
    fflush(stdout);
    return 0;
}