#include "fft/open.h"

/* Stores a colour through a pointer, one statement per source line as the
 * libgpu setRGB0 family does. The pointer is load-bearing: the target forms
 * the colour's address first (pool + 0x20 + index * 36) and stores at 0/1/2
 * from it, where a plain `.color.r` member store addresses each byte
 * absolutely. */
#define OPEN_SET_RGB(color, red, green, blue) ((color)->r = (red), (color)->g = (green), (color)->b = (blue))

/* Appends the next free 36-byte render record to `list`, resets it and
 * returns its pool index. */
s32 open_gfx_append_render_record_36(open_render_record_t** list, s32* count) {
    list[*count] = (open_render_record_t*)&g_open_gfx_render_records_36[g_open_gfx_next_render_record_36];
    g_open_gfx_render_records_36[g_open_gfx_next_render_record_36].flags &= ~0x38;
    g_open_gfx_render_records_36[g_open_gfx_next_render_record_36].palette = 0;
    OPEN_SET_RGB(&g_open_gfx_render_records_36[g_open_gfx_next_render_record_36].color, 0x80, 0x80, 0x80);
    (*count)++;
    return g_open_gfx_next_render_record_36++;
}
