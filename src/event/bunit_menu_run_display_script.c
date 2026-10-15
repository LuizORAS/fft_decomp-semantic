#include "fft/event_bunit.h"
#include "psx/types.h"

/* Run a display script (menu_script_opcode_e) until BUNIT_MENU_SCRIPT_END (0x1c): clear the draw
 * priority, its temporary-draw-area lock and semi-transparency, keep `flags` as the input the handlers
 * read (g_bunit_menu_display_script_input), then dispatch each command through
 * g_bunit_menu_script_handlers, which return the next command. Twin of world_menu_run_display_script. */
void bunit_menu_run_display_script(u8* data, s32 flags) {
    g_bunit_gfx_otag_index = 0;
    g_bunit_gfx_semitrans_enabled = 0;
    g_bunit_menu_display_script_input = flags;
    g_bunit_gfx_otag_index_locked = 0;
    if (data[0] != BUNIT_MENU_SCRIPT_END) {
        do {
            data = g_bunit_menu_script_handlers[data[0]](data);
        } while (data[0] != BUNIT_MENU_SCRIPT_END);
    }
}
