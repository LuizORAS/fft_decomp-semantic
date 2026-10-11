#include "fft/world.h"
#include "psx/types.h"

/* Run a menu display script from its first opcode until the 0x1c terminator,
 * after clearing the shared draw priority, semi-transparency, palette-mode
 * and temporary-draw-area state. */
void world_menu_run_display_script(menu_instruction_t* script, s32 input) {
    menu_instruction_t* instruction = script;

    g_world_menu_draw_priority = 0;
    g_world_menu_semi_trans = 0;
    g_world_menu_display_script_input = input;
    g_world_menu_temporary_draw_area_active = 0;
    if (instruction->header.opcode != 0x1c) {
        do {
            instruction = g_world_menu_script_handlers[instruction->header.opcode](instruction);
        } while (instruction->header.opcode != 0x1c);
    }
}
