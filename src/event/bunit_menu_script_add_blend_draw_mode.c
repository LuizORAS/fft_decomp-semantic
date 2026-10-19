#include "fft/event_bunit.h"
#include "fft/menu.h"

/* Stream command: queue a draw-mode primitive at the current ordering-table
 * entry, selecting the texture page at (0x100, 0) with blend rate
 * data->blend.blend_rate. Returns the next command pointer. */
menu_instruction_t* bunit_menu_script_add_blend_draw_mode(menu_instruction_t* data) {
    menu_gfx_enqueue_draw_mode(0, 0, GetTPage(0, data->blend.blend_rate, 0x100, 0), 0, g_menu_gfx_otag_index);
    return menu_next_instruction(data);
}
