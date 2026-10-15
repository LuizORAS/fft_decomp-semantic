#include "fft/event_equip.h"
#include "psx/types.h"

/* Opcode 0x0e (MENU_SCRIPT_OP_SEMI_TRANS): byte 3 turns semi-transparency on or off for the
 * primitives the following commands queue (g_equip_gfx_semitransparency). Returns the next command. */
const u8* equip_menu_script_set_semi_trans(const u8* data) {
    g_equip_gfx_semitransparency = data[3];
    return data + data[1];
}
