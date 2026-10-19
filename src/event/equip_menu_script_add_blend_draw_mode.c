#include "fft/event_equip.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Enqueue a draw-mode primitive for the command's texture page and return the
 * next command. */
menu_instruction_t* equip_menu_script_add_blend_draw_mode(menu_instruction_t* command) {
    s32 tpage = GetTPage(0, command->blend.blend_rate, 0x100, 0);
    menu_gfx_enqueue_draw_mode(0, 0, tpage, 0, g_menu_gfx_otag_index);
    return menu_next_instruction(command);
}
