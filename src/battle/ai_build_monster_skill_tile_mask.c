#include "fft/battle_ai.h"
#include "fft/battle_map.h"
#include "fft/battle_unit.h"

/* The target loads 0xff as 255, not -1, for the first 2 directions. */
/*
 * Build the targetable-tile mask around same-team Monster Skill holders.
 *
 * Only the low map level is visited; the high-level mask remains cleared.
 * Tile base heights must differ by at most 2; slope and depth are ignored.
 */
void ai_build_monster_skill_tile_mask(void) {
    ai_data_t* ai = &g_ai_data_base;
    battle_stats_t* unit;
    s32 i;
    s32 direction;
    s32 x;
    s32 y;
    s32 level;
    s32 height;
    u16* row;
    u32 level_base;

    for (level = 0; level < AI_GRID_ROW_COUNT; level++) {
        ai->targetable_tiles[0][level] = 0;
        ai->targetable_tiles[1][level] = 0;
    }
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
        height = g_map_tile_data[unit->position.higher_elevation * MAP_TILES_PER_LAYER
            + unit->position.y * ai->map_max_x + unit->x]
                     .height;
        for (direction = 0; direction < AI_CARDINAL_DIRECTION_COUNT; direction++) {
            x = unit->x + g_ai_facing_tile_offsets.by_direction[direction][0];
            if (x < 0 || x >= ai->map_max_x)
                continue;
            y = unit->position.y + g_ai_facing_tile_offsets.by_direction[direction][1];
            if (y < 0 || y >= ai->map_max_y)
                continue;
            for (level = 0; level <= 0; level++) {
                if ((g_map_tile_data[(level * MAP_TILES_PER_LAYER) + y * ai->map_max_x + x].height - height) >= -2
                    && (g_map_tile_data[(level * MAP_TILES_PER_LAYER) + y * ai->map_max_x + x].height - height) <= 2) {
                    /* A typed level-row pointer folds the field offset into
                     * the induction value. Preserve the target's AI-base
                     * cursor with a member-derived displacement. */
                    level_base = level * sizeof(ai->targetable_tiles[0]) + (u32)ai;
                    row = (u16*)(level_base + ((u32)&ai->targetable_tiles - (u32)ai));
                    row[y] |= 0x8000 >> x;
                }
            }
        }
    }
}
