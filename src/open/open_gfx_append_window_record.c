#include "fft/open.h"

/* Appends the next free 56-byte render record to `list`, resets it and
 * returns its pool index. */
s32 open_gfx_append_window_record(void** list, s32* count) {
    list[*count] = &g_open_gfx_render_records_56[g_open_gfx_next_render_record_56];
    g_open_gfx_render_records_56[g_open_gfx_next_render_record_56].flags &= ~(WLDCORE_DISPLAY_OBJECT_8BIT_CLUT
        | WLDCORE_DISPLAY_OBJECT_BLANK | WLDCORE_DISPLAY_OBJECT_HIDDEN | WLDCORE_DISPLAY_OBJECT_SEMI_TRANSPARENT);
    g_open_gfx_render_records_56[g_open_gfx_next_render_record_56].palette = 0;
    /* Direct color members change the shared base and add four instructions. */
    (&g_open_gfx_render_records_56[g_open_gfx_next_render_record_56].color)->r = 0x80;
    (&g_open_gfx_render_records_56[g_open_gfx_next_render_record_56].color)->g = 0x80;
    (&g_open_gfx_render_records_56[g_open_gfx_next_render_record_56].color)->b = 0x80;
    g_open_gfx_render_records_56[g_open_gfx_next_render_record_56].pop_in_step = 0;
    (*count)++;
    return g_open_gfx_next_render_record_56++;
}
