#include "fft/event_bunit.h"
#include "psx/types.h"

/* Stream command: queue a draw-mode primitive at the current ordering-table
 * entry, selecting the texture page at (0x100, 0) with blend rate data[2].
 * Returns the next command pointer. */
u8* bunit_menu_script_add_blend_draw_mode(u8* data) {
    bunit_gfx_enqueue_draw_mode(0, 0, GetTPage(0, data[2], 0x100, 0) & 0xFFFF, 0, g_bunit_gfx_otag_index);
    return data + data[1];
}
