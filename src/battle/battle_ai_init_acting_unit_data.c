#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/* Fill the workspace's acting-unit fields for the unit whose turn is planned: its record and decision
 * pointers, tile, compact id, team, the clock ticks until it acts
 * (battle_ai_calculate_clockticks_until_unit_acts) and its Move, a rider's being its mount's. */
void battle_ai_init_acting_unit_data(void) {
    battle_ai_data_t* ai;
    battle_stats_t* unit;
    battle_stats_t* acting;
    s32 mount_info;
    s32 id;

    ai = &g_battle_ai_data_base;
    id = g_battle_ai_data_base.acting_unit_id;
    unit = &g_battle_unit_stats[id];
    g_battle_ai_data_base.acting_unit = unit;
    g_battle_ai_data_base.acting_unit_decision = &ai->unit_decisions[id];
    g_battle_ai_data_base.acting_unit_coords.fields.x = unit->x;
    g_battle_ai_data_base.acting_unit_coords.fields.y = unit->position.y;
    g_battle_ai_data_base.acting_unit_coords.fields.elevation = unit->position.higher_elevation;
    g_battle_ai_data_base.acting_unit_coords.fields.zero = 0;
    g_battle_ai_data_base.acting_unit_compact_index = ai->unit_compact_indices[id];
    g_battle_ai_data_base.acting_unit_team = unit->team_flags & BATTLE_TEAM_MASK;
    g_battle_ai_data_base.acting_unit_remaining_clockticks = battle_ai_calculate_clockticks_until_unit_acts(unit);
    acting = g_battle_ai_data_base.acting_unit;
    mount_info = acting->mount_info;
    if (mount_info & BATTLE_MOUNT_INFO_FLAG_RIDER) {
        g_battle_ai_data_base.acting_unit_move
            = g_battle_unit_stats[mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK].move;
    } else {
        g_battle_ai_data_base.acting_unit_move = acting->move;
    }
    ai->current_action.coords.fields.zero = 0;
    ai->current_action.command.actor_id = ai->acting_unit_id;
}
