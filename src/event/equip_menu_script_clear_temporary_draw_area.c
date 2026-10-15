#include "fft/event_equip.h"
#include "psx/types.h"

/* Stream command closing a zoomed window: when the zoom transition queued a
 * cropped draw area, queue the full draw area again at the next
 * ordering-table entry and clear the flag. */
u8* equip_menu_script_clear_temporary_draw_area(u8* stream) {
    if (g_equip_gfx_zoom_draw_area_active != 0) {
        s32 otag_index = g_equip_gfx_sprite_ot_index;
        g_equip_gfx_draw_area.y = 0;
        equip_gfx_enqueue_draw_area(&g_equip_gfx_draw_area, otag_index + 1);
        g_equip_gfx_zoom_draw_area_active = 0;
    }
    return stream + stream[1];
}
