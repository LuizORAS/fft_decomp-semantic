#include "fft/event_bunit.h"
#include "psx/types.h"

/* Queue a draw mode with the requested blend rate at the current OT entry. */
menu_instruction_t* bunit_cmd_enqueue_draw_mode_handler(menu_instruction_t* data) {
    bunit_gfx_enqueue_draw_mode(0, 0, GetTPage(0, data->blend.blend_rate, 0x100, 0), 0, g_bunit_gfx_otag_index);
    return menu_next_instruction(data);
}
