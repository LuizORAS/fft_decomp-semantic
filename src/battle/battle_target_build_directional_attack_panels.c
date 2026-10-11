#include "fft/battle.h"

/* Narrow an ability's panels to the straight lines that leave the acting unit
 * toward its target: mode 1 for a linear attack, mode 3 for a three-direction
 * attack.
 *
 * On each axis where the target tile differs from the unit's, the line in that
 * direction is kept; mode 3 also keeps the two perpendicular lines. Every panel
 * off the kept lines loses its range, and the marks that identified the lines are
 * cleared. The call also turns on nearest-first ordering of the target list
 * (g_battle_sort_targets_nearest_first). */
void battle_target_build_directional_attack_panels(const battle_action_command_t* source, u8 mode) {
    battle_action_command_t action;
    battle_stats_t* unit;
    battle_target_panel_t* panel;
    s32 target_x;
    s32 target_y;
    s32 x;
    s32 y;
    s32 i;

    main_util_copy_action_data(source, &action);
    g_battle_sort_targets_nearest_first = 1;
    unit = &g_battle_unit_stats[action.actor_id];
    x = unit->x;
    y = unit->position.y;
    target_x = action.target_x;
    target_y = action.target_y;
    if (x < target_x) {
        battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_EAST, x, y);
        if (mode == 3) {
            battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_NORTH, x, y);
            battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_SOUTH, x, y);
        }
    }
    if (target_x < x) {
        battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_WEST, x, y);
        if (mode == 3) {
            battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_NORTH, x, y);
            battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_SOUTH, x, y);
        }
    }
    if (y < target_y) {
        battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_NORTH, x, y);
        if (mode == 3) {
            battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_EAST, x, y);
            battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_WEST, x, y);
        }
    }
    if (target_y < y) {
        battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_SOUTH, x, y);
        if (mode == 3) {
            battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_WEST, x, y);
            battle_target_calculate_linear_attack_tiles(BATTLE_TARGET_LINE_EAST, x, y);
        }
    }
    for (i = 0; i < 0x200; i++) {
        panel = &g_battle_target_panels[i];
        if (panel->mark == 0) {
            panel->remaining_range = 0;
        } else {
            panel->mark = 0;
        }
    }
}
