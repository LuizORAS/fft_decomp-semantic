#include "fft/open.h"

/* Draw the listed render records into the ordering table. A record with BLANK or
 * HIDDEN set is skipped; ANIMATED selects the 36-byte layout and IMAGE the
 * 56-byte one. Nothing is drawn while
 * OPEN_SYSTEM_RUNTIME_FLAG_RENDER_RECORDS_HIDDEN is set, which no code in
 * OPEN.BIN does. */
void open_gfx_draw_render_record_pointer_list(u32* otag, open_render_record_t** records, s32 count) {
    s32 i;
    open_render_record_t* record;
    u32 flags;

    if ((g_open_system_runtime_flags & OPEN_SYSTEM_RUNTIME_FLAG_RENDER_RECORDS_HIDDEN) != 0) {
        return;
    }

    for (i = 0; i < count; i++) {
        record = records[i];
        flags = record->flags;
        /* Flags bits 0 and 1 select the record layout behind the common prefix. */
        if ((flags & (WLDCORE_DISPLAY_OBJECT_BLANK | WLDCORE_DISPLAY_OBJECT_HIDDEN)) == 0) {
            if ((flags & WLDCORE_DISPLAY_OBJECT_ANIMATED) != 0) {
                open_gfx_draw_animated_render_record((world_animated_render_record_t*)record, otag);
            } else if ((flags & WLDCORE_DISPLAY_OBJECT_IMAGE) != 0) {
                open_gfx_draw_window_record((open_render_record_56_t*)record, otag);
            }
        }
    }
}
