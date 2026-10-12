#include "fft/battle.h"

/*
 * Remember a triggered trap unless its tile is already in the cache.
 *
 * Current coordinates and selected-action defaults are updated even when the
 * list is full. Equality includes all four bytes; x == 0xff marks an empty slot.
 */
void battle_ai_record_triggered_trap_coords(s32 x, s32 y, s32 level) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 i;

    g_battle_ai_data_base.considered_unit_coords.bytes.x = x;
    g_battle_ai_data_base.considered_unit_coords.bytes.y = y;
    g_battle_ai_data_base.considered_unit_coords.bytes.elevation = level;
    g_battle_ai_data_base.considered_unit_coords.bytes.zero = 0;
    battle_ai_init_selected_action();
    for (i = 0; i < BATTLE_AI_KNOWN_TRAP_COUNT; i++) {
        if (ai->known_trap_coords[i].bytes.x == 0xff) {
            ai->known_trap_coords[i] = ai->considered_unit_coords;
            break;
        }
        if (ai->known_trap_coords[i].word == ai->considered_unit_coords.word) {
            break;
        }
    }
}
