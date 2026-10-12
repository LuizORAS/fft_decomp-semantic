#include "fft/battle_ai.h"
#include "fft/battle_map.h"
#include "fft/battle_unit.h"

/*
 * Initialize AI workspace defaults and present-unit indexing.
 *
 * Average maximum HP includes all present slots and is capped at 255.
 */
void ai_init_workspace(void) {
    s32 i = 0;
    s32 present_count = 0;
    s32 total_max_hp = 0;
    ai_data_t* ai = &g_ai_data_base;

    g_ai_workspace = ai;
    g_ai_unit_snapshots = g_ai_unit_snapshot_storage;
    ai->resume_pending = 0;
    ai->selected_action.rank.decided = 0;
    ai->selected_action.coords.fields.zero = 0;
    ai->hardcoded_status_abilities[0].skillset = SKILLSET_ID_BLOOD_SUCK_STATUS;
    ai->hardcoded_status_abilities[0].packed_id = ABILITY_ID_BLOOD_SUCK;
    ai->hardcoded_status_abilities[0].usage.flags = AI_ABILITY_ENTRY_USABLE | AI_ABILITY_ENTRY_EXHAUSTIVE_ORIGIN_SCAN;
    ai->hardcoded_status_abilities[1].skillset = SKILLSET_ID_FROG_ATTACK;
    ai->hardcoded_status_abilities[1].packed_id = ABILITY_ID_FROG_ATTACK;
    ai->hardcoded_status_abilities[1].usage.flags = AI_ABILITY_ENTRY_USABLE | AI_ABILITY_ENTRY_EXHAUSTIVE_ORIGIN_SCAN;
    ai->present_unit_average_max_hp = 0;
    ai->map_max_x = g_map_max_x;
    ai->map_max_y = g_map_max_y;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        ai->unit_compact_indices[i] = 0xff;
        if (g_battle_unit_stats[i].entd_slot != BATTLE_ENTD_SLOT_NONE) {
            ai->unit_compact_indices[i] = present_count;
            ai_init_unit_abilities(i);
            present_count++;
            total_max_hp += g_battle_unit_stats[i].max_hp;
        }
    }
    if (present_count != 0) {
        s32 average = total_max_hp / present_count;
        if (average >= 256) {
            ai->present_unit_average_max_hp = 255;
        } else {
            ai->present_unit_average_max_hp = average;
        }
    }
    {
        s32 empty = 0xff;
        for (i = AI_KNOWN_TRAP_COUNT - 1; i >= 0; i--) {
            ai->known_trap_coords[i].fields.x = empty;
        }
    }
    ai->throw_ability_id = ABILITY_ID_THROW_KNIFE;
    ai->throw_weapon_id = ITEM_ID_DAGGER;
}
