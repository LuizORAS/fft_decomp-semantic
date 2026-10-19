#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Redirect a reflected spell: the unit holding Reflect casts the last attack it received again
 * (as Black Magic, reaction 5) at the tile as far beyond it as the caster is in front of it
 * (battle_map_resolve_rotated_offset_layer). Returns -1 when the caster stands on the unit's
 * square or that tile is off the map or blocked. */
s32 battle_target_apply_reflect(battle_stats_t* unit) {
    action_command_t* act;
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
    act->target_position.vx = x;
    act->target_position.vz = y;
    act->target_position.vy = z;
    return 0;
}
