#include "fft/open.h"

/* Appends the next free 36-byte render record to `list`, resets it and
 * returns its pool index. */
s32 open_gfx_append_animated_render_record(open_render_record_t** list, s32* count) {
    list[*count] = (open_render_record_t*)&g_open_gfx_render_records_36[g_open_gfx_next_render_record_36];
    g_open_gfx_render_records_36[g_open_gfx_next_render_record_36].flags
        &= ~(WLDCORE_DISPLAY_OBJECT_BLANK | WLDCORE_DISPLAY_OBJECT_HIDDEN | WLDCORE_DISPLAY_OBJECT_SEMI_TRANSPARENT);
    g_open_gfx_render_records_36[g_open_gfx_next_render_record_36].palette = 0;
    /* Direct color members change the shared base and add four instructions. */
    (&g_open_gfx_render_records_36[g_open_gfx_next_render_record_36].color)->r = 0x80;
    (&g_open_gfx_render_records_36[g_open_gfx_next_render_record_36].color)->g = 0x80;
    (&g_open_gfx_render_records_36[g_open_gfx_next_render_record_36].color)->b = 0x80;
    (*count)++;
    return g_open_gfx_next_render_record_36++;
}
