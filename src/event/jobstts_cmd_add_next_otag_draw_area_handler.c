#include "fft/event_jobstts.h"

/* Stream command: when the ordering-table index is not locked, reset the
 * shared draw area's y to 0 and enqueue it at the next ordering-table
 * entry. */
menu_instruction_t* jobstts_cmd_add_next_otag_draw_area_handler(menu_instruction_t* data) {
    if (g_jobstts_gfx_otag_index_locked == 0) {
        g_jobstts_gfx_draw_area.y = 0;
        jobstts_gfx_enqueue_draw_area(&g_jobstts_gfx_draw_area, g_jobstts_gfx_otag_index + 1);
    }
    return menu_next_instruction(data);
}
