#include "fft/battle_ai.h"
#include "fft/battle_map.h"
#include "fft/battle_unit.h"

/* Return 1 when the considered ability may be used from coords: not water 2 or more deep for a unit
 * with the water penalty; for Elemental, terrain that gives this ability
 * (battle_ai_can_elemental_ability_be_used); for an ability that needs Monster Skill, a same-team
 * holder that can react on or next to the tile, within 2 levels. Range and targets are checked
 * elsewhere. */
s32 battle_ai_can_use_ability_at_coords(const battle_ai_coords_t* coords) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 i;
    s32 dx;
    s32 dy;
    s32 height;
    battle_stats_t* unit;

    if (g_battle_ai_data_base.water_penalty) {
        s32 depth = g_battle_map_tile_data[coords->fields.y * g_battle_ai_data_base.map_max_x + coords->fields.x
            + (coords->fields.elevation * MAP_TILES_PER_LAYER)]
                        .depth_half_height.depth;
        if (depth >= 2)
            return 0;
    }
    if (g_main_action_menu_types_by_skillset[ai->considered_ability.skillset] == ACTION_MENU_TYPE_ELEMENTS)
        return battle_ai_can_elemental_ability_be_used(coords);
    if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_REQUIRES_MONSTER_SKILL) {
        for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
            unit = &g_battle_unit_stats[i];
            if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE)
                continue;
            if (!(unit->ability_flags.support[3] & BATTLE_SUPPORT_SET_4_MONSTER_SKILL))
                continue;
            if ((unit->initial_team_flags & BATTLE_TEAM_MASK) != ai->acting_unit_team)
                continue;
            if (main_unit_has_status_in_set(unit, MAIN_STATUS_CHECK_SET_PREVENT_REACTION))
                continue;
            dx = unit->x - coords->fields.x;
            if (dx < 0)
                dx = -dx;
            dy = unit->position.y - coords->fields.y;
            if (dy < 0)
                dy = -dy;
            if (dx + dy < 2) {
                height = battle_ai_get_unit_height_above_coords(coords, unit);
                if (height >= -2 && height <= 2)
                    return 1;
            }
        }
        return 0;
    }
    return 1;
}
