#include "fft/battle.h"
#include "psx/types.h"

/* Aim a reflected spell: the unit casts the ability it received, under the
 * Black Magic skillset, at the tile as far beyond it as the caster is in front
 * of it. -1 when the caster stands on the unit's tile or that tile is off the
 * map or blocked. */
s32 battle_target_apply_reflect(battle_stats_t* unit) {
    battle_unit_action_command_t* act;
    s32 x;
    s32 y;
    s32 z;
    s32 dx;
    s32 dy;
    s32 last_attack;

    act = &unit->action_command;
    dx = unit->x - g_battle_acting_unit_x;
    dy = unit->position.y - g_battle_acting_unit_y;
    if (dx == 0 && dy == 0) {
        return -1;
    }
    if (battle_map_resolve_rotated_offset_layer(unit, dx, dy, 0, &x, &y, &z) != 0) {
        return -1;
    }
    act->skillset_id = SKILLSET_ID_BLACK_MAGIC;
    last_attack = unit->action.last_received_attack;
    act->targeting_type = BATTLE_TARGETING_TYPE_TILE;
    act->target_id = 0;
    act->ability_id = last_attack;
    act->target_x = x;
    act->target_y = y;
    act->target_elevation = z;
    return 0;
}
