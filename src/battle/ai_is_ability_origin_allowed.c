#include "fft/battle_ai.h"
#include "fft/battle_map.h"
#include "fft/battle_unit.h"

/*
 * Check depth, terrain, and Monster Skill restrictions at an AI origin.
 *
 * Return 1 when these checks permit use, otherwise 0. Geomancy delegates
 * directly to its terrain check; this is not a complete ability-use test.
 */
s32 ai_is_ability_origin_allowed(const ai_coords_t* coords) {
    ai_data_t* ai = &g_ai_data_base;
    s32 i;
    s32 dx;
    s32 dy;
    s32 height;
    unit_stats_t* unit;

    if (g_ai_data_base.water_penalty) {
        s32 depth = g_map_tile_data[coords->fields.y * g_ai_data_base.map_max_x + coords->fields.x
            + (coords->fields.elevation * MAP_TILES_PER_LAYER)]
                        .depth_half_height.depth;
        if (depth >= 2)
            return 0;
    }
    if (g_main_action_menu_types_by_skillset[ai->considered_ability.skillset] == ACTION_MENU_TYPE_ELEMENTS)
        return ai_can_use_geomancy(coords);
    if (ai->considered_ability.ai_flags.word & AI_ABILITY_REQUIRES_MONSTER_SKILL) {
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
                height = ai_calculate_height_difference_between_units(coords, unit);
                if (height >= -2 && height <= 2)
                    return 1;
            }
        }
        return 0;
    }
    return 1;
}
