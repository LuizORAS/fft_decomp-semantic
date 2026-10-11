#include "fft/event_equip.h"
#include "psx/types.h"

/* Stream command: when the zoom transition has not queued a cropped draw
 * area, reset the shared draw area's y to 0 and enqueue it at the next
 * ordering-table entry. */
u8* equip_cmd_add_next_otag_draw_area_handler(u8* stream) {
    if (g_equip_gfx_zoom_draw_area_active == 0) {
        s32 otag_index = g_equip_gfx_sprite_ot_index;
        g_equip_gfx_draw_area.y = 0;
        equip_gfx_enqueue_draw_area(&g_equip_gfx_draw_area, otag_index + 1);
    }
    return stream + stream[1];
}
