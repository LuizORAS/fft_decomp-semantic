/* LIBGPU 80023288-800235ac. */
#include "psx/libgpu.h"

u32* FntFlush(int id) {
    /* The retail gives each local an eight-byte stack slot. */
    /* Volatile preserves the packet's original stack store/reload. */
    DR_MODE* volatile packet[2];
    int maximum_x[2];
    int bottom[2];
    /* The a2 pin retains the original initialization-store scheduling. */
    register int initial_color __asm__("$6") = 128;
    typedef struct {
        union {
            int value;
            u8 low_byte;
        } color;
        int _unused_04; /* The unused word preserves the eight-byte local slot. */
    } font_color_slot_t;
    font_color_slot_t red;
    font_color_slot_t green;
    font_color_slot_t blue;
    psyq_font_window_t* window;
    char* text;
    /* The s2 pin retains AddPrim argument and sprite-advancement ordering. */
    register SPRT_8* sprite __asm__("$18");
    int remaining;
    int x;
    int y;
    /* These pins retain the window-bound and wrapping register allocation. */
    register int right __asm__("$23");
    register int no_wrap __asm__("$22");
    int newline;
    int glyph;
    /* The a1 pin retains the signed glyph-quotient scheduling. */
    register int row __asm__("$5");
    int coordinate;
    /* These temporaries retain the glyph loop's interleaved loads and stores. */
    register int value __asm__("$6");
    register int first __asm__("$2");
    int second;
    u32* result;
    /* The v1 pin retains the final text-pointer load before the tag result. */
    register char* result_text __asm__("$3");
    SPRT_8* argument_sprite;
    red.color.value = initial_color;
    green.color.value = initial_color;
    /* Keep these two stores ahead of the prologue's register spills. */
    __asm__("" : : "m"(red.color.value), "m"(green.color.value));
    maximum_x[0] = 0;
    blue.color.value = initial_color;
    if (id < 0 || id >= g_psyq_gpu_font_window_count) {
        id = g_psyq_gpu_default_font_window;
        if (!g_psyq_gpu_font_windows[id].text)
            return 0;
    }
    window = &g_psyq_gpu_font_windows[id];
    value = (int)&window->mode;
    packet[0] = (DR_MODE*)value;
    coordinate = (int)packet[0];

    text = window->text;
    remaining = window->character_limit;
    x = window->tile.x0;
    y = window->tile.y0;
    first = window->tile.w;
    /* Preserve the independent width/height loads and their retail order. */
    __asm__ volatile("" : "=r"(first) : "0"(first) : "$3");
    second = window->tile.h;
    bottom[0] = y + second;
    sprite = window->sprites;
    no_wrap = window->wrap_disabled;
    right = x + first;
    TermPrim((DR_MODE*)coordinate);
    while (*text && remaining) {
        newline = 0;
        switch (*text) {
        case '~':
            text++;
            if (*text == 'c') {
                text++;
                first = *text++;
                second = *text++;
                first = (first - '0') << 4;
                second = second - '0';
                red.color.value = first;
                first = *text;
                second = second << 4;
                green.color.value = second;
                first = (first - '0') << 4;
                blue.color.value = first;
            }
            break;
        case '\t':
            x += 32;
            goto check_wrap;
        default:
            /* Retain the retail's repeated glyph-byte reads. */
            if ((unsigned)(*text - 'a') < 26) {
                first = *(volatile char*)text;

                glyph = first - 64;
            } else {
                first = *(volatile char*)text;

                glyph = first - 32;
            }
            first = glyph;
            if (glyph < 0)
                first = glyph + 15;

            row = first >> 4;
            first = row << 4;
            first = glyph - first;
            coordinate = first << 3;
            first = row << 3;
            sprite->u0 = coordinate;
            sprite->v0 = first;
            sprite->x0 = x;
            sprite->y0 = y;
            /* Keep XY/UV stores ahead of the color-byte loads. */
            __asm__ volatile("" : : : "memory");
            value = red.color.low_byte;
            /* Keep the AddPrim argument copy between this color load/store. */
            __asm__("" : "=r"(value) : "0"(value) : "$5");
            argument_sprite = sprite;
            /* Retain the argument copy ahead of the red-byte store. */
            __asm__ volatile("" : "=r"(argument_sprite) : "0"(argument_sprite));
            sprite->r0 = value;
            value = green.color.low_byte;
            sprite->g0 = value;
            value = blue.color.low_byte;
            sprite->b0 = value;
            sprite++;
            AddPrim((u32*)packet[0], argument_sprite);
        case ' ':
            x += 8;
        check_wrap:
            if (x >= right && !no_wrap)
                newline = 1;
            break;
        case '\n':
            newline = 1;
            break;
        }
        if (newline) {
            value = maximum_x[0];
            first = value < x;
            if (first)
                maximum_x[0] = x;
            y += 8;
            x = window->tile.x0;
            value = bottom[0];
            first = y < value;
            if (!first)
                break;
        }
        text++;
        remaining--;
    }
    if (window->tile.code) {
        AddPrim((u32*)packet[0], &window->tile);
        if (no_wrap) {
            /* Unsigned lvalue views preserve the original lhu instructions. */
            first = *(u16*)&window->tile.x0;
            value = maximum_x[0];
            second = *(u16*)&window->tile.y0;
            first = value - first;
            second -= 8;
            second = y - second;
            window->tile.w = first;
            window->tile.h = second;
        }
    }
    DrawOTag((u32*)packet[0]);
    result_text = window->text;
    result = (u32*)packet[0];
    window->text_count = 0;
    *result_text = 0;
    return result;
}
