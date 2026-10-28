#include "fft/event_equip.h"
#include "fft/menu.h"
#include "fft/menu_script.h"
#include "psx/types.h"

/* Run a display script (menu_script_opcode_e) until EQUIP_MENU_SCRIPT_END (0x19): clear the draw
 * priority, its temporary-draw-area flag and semi-transparency, keep `input` as the input the handlers read
 * (g_equip_menu_display_script_input), then dispatch each command through menu_script_handlers,
 * which return the next command. Twin of world_menu_run_display_script. */
void equip_menu_run_display_script(menu_instruction_t* instruction, s32 input) {
    g_menu_gfx_otag_index = 0;
    g_menu_gfx_semitransparency = 0;
    g_equip_menu_display_script_input = input;
    g_equip_gfx_zoom_draw_area_active = 0;
    if (instruction->header.opcode != EQUIP_MENU_SCRIPT_END) {
        do {
            instruction = menu_script_handlers[instruction->header.opcode](instruction);
        } while (instruction->header.opcode != EQUIP_MENU_SCRIPT_END);
    }
}
