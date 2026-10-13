#include "fft/event_equip.h"
#include "fft/menu_script.h"
#include "psx/types.h"

/* Dispatch every command of a sprite command instruction through the opcode
   handler table until the 0x19 end-of-instruction opcode. */
void equip_cmd_run_stream(menu_instruction_t* instruction, s32 input) {
    g_equip_gfx_sprite_ot_index = 0;
    g_equip_gfx_semitransparency = 0;
    g_equip_cmd_stream_input = input;
    g_equip_gfx_zoom_draw_area_active = 0;
    if (instruction->header.opcode != EQUIP_CMD_END) {
        do {
            instruction = g_menu_cmd_handlers[instruction->header.opcode](instruction);
        } while (instruction->header.opcode != EQUIP_CMD_END);
    }
}
