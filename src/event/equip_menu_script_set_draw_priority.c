#include "fft/event_equip.h"
#include "psx/types.h"

/* Opcode 0x0f (MENU_SCRIPT_OP_DRAW_PRIORITY): byte 3 becomes the draw priority, the ordering-table
 * index of the primitives the following commands queue (g_equip_gfx_sprite_ot_index). Returns the next
 * command. */
const u8* equip_menu_script_set_draw_priority(const u8* data) {
    g_equip_gfx_sprite_ot_index = data[3];
    return data + data[1];
}
