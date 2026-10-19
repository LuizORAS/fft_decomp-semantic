#include "fft/battle.h"
#include "fft/battle_target.h"

/* Linear and three-direction abilities: keep in the area only the tiles on the line from the unit
 * toward the target (mode 1), or on that line and the two at right angles to it (mode 3), and have
 * the hit units listed nearest first (g_battle_sort_targets_nearest_first). */
void battle_target_build_directional_attack_panels(const action_command_t* source, u8 mode) {
    action_command_t action;
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
    target_x = action.target_position.vx;
    target_y = action.target_position.vz;
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
