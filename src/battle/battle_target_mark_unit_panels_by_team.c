#include "fft/battle.h"
#include "psx/types.h"

/* Mark as a choice the panel under each ally (ALLY_UNIT_TILES) or enemy (ENEMY_UNIT_TILES) unit.
 * The loop never advances `unit`, so it only ever marks the caster's own tile, after its own team
 * test; no retail ability sets these flags (QUIRKS.md). */
void battle_target_mark_unit_panels_by_team(battle_stats_t* unit, s32 flags) {
    battle_target_panel_t* panel;
    s32 i;
    s32 mark_allies;
    s32 mark_enemies;
    s32 team_diff;
    s16 team_diff_copy;
    s32 team;

    mark_allies = flags & ABILITY_SECONDARY_FLAG_1_ALLY_UNIT_TILES;
    mark_enemies = flags & ABILITY_SECONDARY_FLAG_1_ENEMY_UNIT_TILES;
    team = unit->team_flags;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE) {
            team_diff = team ^ unit->initial_team_flags;
            team_diff_copy = team_diff;
            panel = &g_battle_target_panels[battle_map_calculate_location(unit)];
            if (mark_allies != 0 && (team_diff & BATTLE_TEAM_MASK) == 0) {
                panel->mark = 1;
            }
            if (mark_enemies != 0 && (team_diff_copy & BATTLE_TEAM_MASK) != 0) {
                panel->mark = 1;
            }
        }
    }
}
