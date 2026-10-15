#include "fft/battle.h"

/* Remember a tile where a unit sprang a trap (battle_move_start_next_post_move_event): add x, y and level
 * to the four sprung-trap tiles unless already there (a full list keeps its four), and reset the
 * selected action so the unit plans again. The AI leaves these tiles out of its reachable tiles until
 * the workspace is set up again (battle_ai_init_workspace). */
void battle_ai_record_sprung_trap(s32 x, s32 y, s32 level) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 i;

    g_battle_ai_data_base.considered_unit_coords.bytes.x = x;
    g_battle_ai_data_base.considered_unit_coords.bytes.y = y;
    g_battle_ai_data_base.considered_unit_coords.bytes.elevation = level;
    g_battle_ai_data_base.considered_unit_coords.bytes.zero = 0;
    battle_ai_init_selected_action();
    for (i = 0; i < 4; i++) {
        if (ai->sprung_trap_tiles[i].bytes.x == 0xff) {
            ai->sprung_trap_tiles[i].word = ai->considered_unit_coords.word;
            break;
        }
        if (ai->sprung_trap_tiles[i].word == ai->considered_unit_coords.word) {
            break;
        }
    }
}
