#include "fft/event_bunit.h"
#include "fft/menu.h"
#include "fft/menu_script.h"
#include "psx/types.h"

/* Run a display script (menu_script_opcode_e) until BUNIT_MENU_SCRIPT_END (0x1c): clear the draw
 * priority, its temporary-draw-area lock and semi-transparency, keep `flags` as the input the handlers
 * read (g_bunit_menu_display_script_input), then dispatch each command through
 * menu_script_handlers, which return the next command. Twin of world_menu_run_display_script. */
void bunit_menu_run_display_script(menu_instruction_t* instruction, s32 flags) {
    g_menu_gfx_otag_index = 0;
    g_menu_gfx_semitransparency = 0;
    g_bunit_menu_display_script_input = flags;
    g_bunit_gfx_otag_index_locked = 0;
    if (instruction->header.opcode != BUNIT_MENU_SCRIPT_END) {
        do {
            instruction = menu_script_handlers[instruction->header.opcode](instruction);
        } while (instruction->header.opcode != BUNIT_MENU_SCRIPT_END);
    }
}
