/* LIBGPU 800235ac-8002398c; switch table 80010570-800105f4. */
#include "psx/libgpu.h"

/* Retail keeps the argument cursor in memory (as sprintf does): the SDK's va_arg reaches it through its address. */
typedef char* va_list;
#define va_start(ap, last) (ap = (char*)&(last) + sizeof(last))
#define va_arg(ap, type)   (*(char**)&(ap) += sizeof(type), *(type*)((ap) - sizeof(type)))

/* Format text into a font window's buffer, like a small printf (%d, %x, %X, %c, %s, %%, widths and zero
 * padding): `first` is the window id, or, when it is not a valid id, the format itself for the default
 * window. Returns the window's character count, or -1 once it is full (QUIRKS.md). The battle debug and heap
 * reports print with it. */
int FntPrint(char* first, ...) {
    char buffer[512];
    va_list ap;
    psyq_font_window_t* window;
    char* format;
    char* p;
    int width;
    int length;
    int c;
    u8 zero_pad;
    unsigned int number;
    int sign;
    va_start(ap, first);
    if ((int)first < 0 || (int)first >= g_psyq_gpu_font_window_count) {
        format = first;
        first = (char*)g_psyq_gpu_default_font_window;
        if (!g_psyq_gpu_font_windows[(int)first].text)
            return -1;
    } else
        format = va_arg(ap, char*);
    window = &g_psyq_gpu_font_windows[(int)first];
    if (window->text_count > window->character_limit)
        return -1;
    for (; (c = *format) != 0; format++) {
        if (c != '%') {
            window->text[window->text_count++] = c;
            if (window->text_count > window->character_limit)
                return -1;
            continue;
        }
        if ((c = *++format) == '%') {
            window->text[window->text_count++] = c;
            if (window->text_count > window->character_limit)
                return -1;
            continue;
        }
        width = 0;
        zero_pad = c == '0';
        while (c >= '0' && c <= '9') {
            width = width * 10 + (c - '0');
            c = *++format;
        }
        if (width <= 0)
            width = 1;
        p = buffer + sizeof(buffer);
        /* No default: an unsupported conversion leaves length unset. */
        switch (c) {
        case 'd':
            number = va_arg(ap, int);
            sign = 0;
            if ((int)number < 0) {
                number = -number;
                sign = '-';
            }
            for (length = 0; length == 0 || number; length++) {
                *--p = number % 10 + '0';
                number /= 10;
            }
            if (sign) {
                *--p = sign;
                length++;
            }
            break;
        case 'x':
        case 'X':
            number = va_arg(ap, unsigned int);
            for (length = 0; length == 0 || number; length++) {
                *--p = g_psyq_gpu_font_hex_digits[number & 15];
                number >>= 4;
            }
            if (zero_pad) {
                while (length < width) {
                    *--p = '0';
                    length++;
                }
            }
            break;
        case 'c':
            *--p = va_arg(ap, int);
            length = 1;
            break;
        case 's':
            p = va_arg(ap, char*);
            length = strlen(p);
            break;
        }
        while (length < width) {
            window->text[window->text_count++] = ' ';
            if (window->text_count > window->character_limit)
                return -1;
            width--;
        }
        while (length--) {
            window->text[window->text_count++] = *p++;
            if (window->text_count > window->character_limit)
                return -1;
        }
    }
    window->text[window->text_count] = 0;
    return window->text_count;
}
