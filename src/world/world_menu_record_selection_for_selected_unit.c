#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_record_selection_for_selected_unit. The typed row subscript
 * g_world_menu_unit_selection_records[i].command_index costs the match here (function offset
 * 0x8): the target multiplies the byte offset into the address before the base register is
 * formed. Keep the byte view. */
void world_menu_record_selection_for_selected_unit(void) {
    s32 i;
    /* Local pointer keeps the row-index address hoisted out of the loop. */
    s16* stored_unit = &g_menu_selected_unit.battle_id;

    for (i = 0; i < 12; i++) {
        if (g_menu_current_id == g_world_menu_command_maps[i].menu_id) {
            ((u8*)g_world_menu_unit_selection_records)[*stored_unit * MENU_UNIT_SELECTION_RECORD_BYTES]
                = g_menu_thread_menu_data[g_menu_current_id].selected_index;
        }
    }
}
