/* LIBGPU 800235ac-8002398c; switch table 80010570-800105f4. */
#include "psx/libgpu.h"

/* first is either a window handle encoded as a pointer or the format string. */
int FntPrint(char* first, ...) {
    char buffer[512];
    char* arguments = (char*)__builtin_next_arg(first);
    psyq_font_window_t* window;
    char* format;
    char* segment;
    int width;
    int length;
    int character;
    char* argument;
    u8 zero_pad;
    /* The shared v0 temporary preserves width-parser and character-argument scheduling. */
    register int value __asm__("$2");
    int percent;
    int selected;
    char* selected_text;
    unsigned int number;
    int sign;
    /* Address-taking stores preserve the retail parameter and cursor stack homes. */
    if ((int)first < 0 || (int)first >= g_psyq_gpu_font_window_count) {
        selected = g_psyq_gpu_default_font_window;
        selected_text = g_psyq_gpu_font_windows[selected].text;

        format = first;
        *(&first) = (char*)selected;
        if (!selected_text)
            return -1;
    } else {
        *(&arguments) = (char*)__builtin_next_arg(first) + sizeof(u32);
        format = *(char**)((char*)__builtin_next_arg(first));
    }
    window = &g_psyq_gpu_font_windows[(int)first];
    if (window->text_count > window->character_limit)
        return -1;
    if (!(character = *format))
        goto finished;
    percent = '%';
    do {
        if (character != percent || (character = *++format) == percent) {
            window->text[window->text_count++] = character;
            if (window->text_count <= window->character_limit) {
                format++;
                continue;
            }
            return -1;
        }
        width = 0;
        value = character == '0';

        zero_pad = value;
        while ((unsigned)(character - '0') < 10) {
            value = width * 10;
            value -= '0';

            width = value + character;
            character = *++format;
        }
        if (width <= 0)
            width = 1;
        segment = buffer + sizeof(buffer);
        /* No default: unsupported conversions leave length undefined in the retail. */
        switch (character) {
        case 'd':
            argument = arguments;
            arguments = argument + sizeof(u32);
            number = *(unsigned int*)argument;
            sign = 0;
            if ((int)number < 0) {
                number = -number;
                sign = '-';
            }
            length = 0;
            while (!length || number) {
                *--segment = number % 10 + '0';
                number /= 10;
                length++;
            }
            if (sign) {
                *--segment = sign;
                length++;
            }
            break;
        case 'x':
        case 'X':
            argument = arguments;
            arguments = argument + sizeof(u32);
            number = *(unsigned int*)argument;
            length = 0;
            while (!length || number) {
                *--segment = g_psyq_gpu_font_hex_digits[number & 15];
                number >>= 4;
                length++;
            }
            value = zero_pad;

            if (value) {
                while (length < width) {
                    *--segment = '0';
                    length++;
                }
            }
            break;
        case 'c':
            value = (int)arguments;
            arguments = (char*)value + sizeof(u32);
            *--segment = *(u8*)value;
            length = 1;
            break;
        case 's':
            argument = arguments;
            arguments = argument + sizeof(u32);
            segment = *(char**)argument;
            length = strlen(segment);
            break;
        }
        while (length < width) {
            window->text[window->text_count++] = ' ';
            if (window->text_count > window->character_limit)
                return -1;
            width--;
        }
        while (--length != -1) {
            window->text[window->text_count++] = *segment++;
            if (window->text_count > window->character_limit)
                return -1;
        }
        format++;
    } while ((character = *format));
finished:
    window->text[window->text_count] = 0;
    return window->text_count;
}
