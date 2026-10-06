#include "fft/battle.h"
#include "psx/types.h"

/* Blaze Gun, Glacier Gun and Blast Gun (item ids 0x4a..0x4c) play the weapon
 * strike even when an ability is used. */
#define IS_ELEMENTAL_GUN(id) (((id) == 0x4a || (id) == 0x4b) || (id) == 0x4c)

/* Apply a strike's results and pick the actor's animation, entering START_ACTION_EXECUTE at 60 fps.
 * The first strike turns the actor toward its target. Each target's pending result applies
 * (battle_action_apply_target_result), and a target whose result is -1 (death) gets a
 * relocation tile (battle_unit_find_relocation_tile). In the action phase an ability (not an
 * elemental gun) shown without a reaction plays its ability animation, and a plain attack or an
 * elemental gun the weapon strike. In the other phases (First Strike, reactions) a knockback strike
 * moves its targets to the knockback tile, and otherwise the same choice applies to the first strike,
 * except after Reflect. */
void battle_action_apply_strike_results(void) {
    battle_unit_misc_data_t* unit;
    battle_unit_misc_data_t* target;
    battle_strike_work_t* action;
    s32 i;

    g_battle_state_vsync_interval = 1;
    g_battle_game_state = BATTLE_GAME_STATE_START_ACTION_EXECUTE;
    unit = battle_unit_get_casting_misc_data();
    unit->state_frame_counter = 0;
    if (unit->continue_attack_count == 0) {
        battle_unit_face_towards_action_target(unit, 0);
    }
    action = (battle_strike_work_t*)&unit->action_18c;
    if (action->target_count != 0) {
        for (i = 0; i < action->target_count; i++) {
            target = battle_unit_get_misc_data_by_battle_id(action->target_list[i]);
            if (target != 0) {
                target->pending_attack_result = battle_action_apply_target_result(target->battle_data->misc_unit_id);
                if (target->pending_attack_result == -1) {
                    battle_unit_find_relocation_tile(target->battle_data->misc_unit_id, &target->dismount);
                }
            }
        }
    }

    if (g_battle_action_phase == 1) {
        if (unit->used_ability_id != 0 && !IS_ELEMENTAL_GUN(unit->used_item_or_weapon_id)
            && action->reaction_occurred == 0) {
            battle_unit_select_attack_animation_for_ability(
                unit, battle_unit_get_misc_data_by_battle_id(action->target_list[0]));
        } else if (unit->used_ability_id == 0 || IS_ELEMENTAL_GUN(unit->used_item_or_weapon_id)) {
            if (action->target_count != 0) {
                battle_unit_select_weapon_attack_animation(
                    unit, battle_unit_get_misc_data_by_battle_id(action->target_list[0]));
            } else {
                battle_unit_select_weapon_attack_animation(unit, 0);
            }
        }
    } else if (unit->used_ability_id == ABILITY_ID_KNOCKBACK) {
        for (i = 0; i < unit->target_count; i++) {
            target = battle_unit_get_misc_data_by_battle_id(action->target_list[i]);
            if (target != 0) {
                target->map_x = unit->target_new_x;
                target->map_y = unit->target_new_y;
                target->map_z = unit->target_new_map_level;
                battle_unit_set_tile_position(target->battle_data->misc_unit_id, target->map_x, target->map_y,
                    target->map_z, target->facing / 1024);
                battle_unit_set_move_and_screen_coords(target);
            }
        }
    } else if (action->reaction_ability_id != ABILITY_ID_REACTION_REFLECT) {
        if (unit->used_ability_id != 0 && !IS_ELEMENTAL_GUN(unit->used_item_or_weapon_id)
            && unit->continue_attack_count == 0) {
            if (action->reaction_occurred == 0) {
                battle_unit_select_attack_animation_for_ability(
                    unit, battle_unit_get_misc_data_by_battle_id(action->target_list[0]));
            }
        } else if (unit->used_ability_id == 0 || IS_ELEMENTAL_GUN(unit->used_item_or_weapon_id)) {
            if (action->target_count != 0) {
                battle_unit_select_weapon_attack_animation(
                    unit, battle_unit_get_misc_data_by_battle_id(action->target_list[0]));
            } else {
                battle_unit_select_weapon_attack_animation(unit, 0);
            }
        }
    }
}
