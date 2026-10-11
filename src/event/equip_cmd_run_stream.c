#include "fft/event_equip.h"
#include "psx/types.h"

/* Dispatch every command of a sprite command stream through the opcode
   handler table until the 0x19 end-of-stream opcode. */
void equip_cmd_run_stream(u8* stream, s32 input) {
    menu_instruction_t* instruction = (menu_instruction_t*)stream;

    g_equip_gfx_sprite_ot_index = 0;
    g_equip_gfx_semitransparency = 0;
    g_equip_cmd_stream_input = input;
    g_equip_gfx_zoom_draw_area_active = 0;
    if (instruction->header.opcode != EQUIP_CMD_END) {
        do {
            instruction = g_equip_cmd_handlers[instruction->header.opcode](instruction);
        } while (instruction->header.opcode != EQUIP_CMD_END);
    }
}
