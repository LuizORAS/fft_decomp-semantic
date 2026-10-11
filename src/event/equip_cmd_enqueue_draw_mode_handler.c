#include "fft/event_equip.h"
#include "psx/types.h"

/* Enqueue a draw-mode primitive for the command's texture page and return the
 * next command. */
menu_instruction_t* equip_cmd_enqueue_draw_mode_handler(menu_instruction_t* command) {
    s32 tpage = GetTPage(0, command->blend.blend_rate, 0x100, 0);
    equip_gfx_enqueue_draw_mode(0, 0, tpage, 0, g_equip_gfx_sprite_ot_index);
    return menu_next_instruction(command);
}
