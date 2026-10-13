#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Stream command: enqueue a draw-area rect (x, y, w, h from the command)
 * unless the zoom transition already emitted one. */
u8* jobstts_menu_script_set_draw_area(u8* data) {
    RECT area;

    if (g_jobstts_gfx_otag_index_locked == 0) {
        area.x = data[2];
        area.y = data[3];
        area.w = data[4];
        area.h = data[5];
        jobstts_gfx_enqueue_draw_area(&area, g_jobstts_gfx_otag_index - 1);
    }
    return data + data[1];
}
