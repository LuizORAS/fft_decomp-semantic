#include "fft/world.h"
#include "psx/types.h"

/* Run a menu display script (menu_script_opcode_e) from its first command until
 * MENU_SCRIPT_OP_WORLD_END, after clearing the shared draw priority, semi-transparency, palette-mode and
 * temporary-draw-area state. Each handler in g_world_menu_script_handlers returns the next command. */
void world_menu_run_display_script(u8* script, s32 input) {
    g_world_menu_draw_priority = 0;
    g_world_menu_semi_trans = 0;
    g_world_menu_display_script_input = input;
    g_world_menu_temporary_draw_area_active = 0;
    if (*script != MENU_SCRIPT_OP_WORLD_END) {
        do {
            script = g_world_menu_script_handlers[*script](script);
        } while (*script != MENU_SCRIPT_OP_WORLD_END);
    }
}
