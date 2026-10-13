#include "fft/battle.h"
#include "fft/battle_action.h"
#include "psx/types.h"

/* Blaze Gun, Glacier Gun and Blast Gun (item ids 0x4a..0x4c) play the weapon
 * strike even when an ability is used. */
#define IS_ELEMENTAL_GUN(id) ((id == ITEM_ID_BLAZE_GUN || id == ITEM_ID_GLACIER_GUN) || id == ITEM_ID_BLAST_GUN)

/* Finalizes the casting unit's action and picks its attack animation.
 *
 * Each target's attack result is resolved first; for Fall (ability 0x200) in
 * the later phase the targets are moved to the knockback destination instead.
 * An ability picks the ability animation unless a reaction occurred, while a
 * plain attack or an elemental gun picks the weapon strike.
 *
 * continue_attack_count is nonzero on the follow-up strikes of a continued
 * attack. */
void action_set_target_coords_and_attacker_anim(void) {
    battle_unit_t* unit;
    battle_unit_t* target;
    battle_strike_work_t* action;
    s32 i;

    g_main_system_animation_speed = 1;
    g_battle_game_state = BATTLE_GAME_STATE_SECONDARY_EFFECT;
    unit = battle_unit_get_casting();
    unit->state_frame_counter = 0;
    if (unit->continue_attack_count == 0) {
        battle_unit_face_towards_action_target(unit, 0);
    }
    action = &unit->strike;
    if (action->target_count != 0) {
        for (i = 0; i < action->target_count; i++) {
            target = battle_unit_get_by_battle_id(action->target_list[i]);
            if (target != 0) {
                target->pending_attack_result
                    = action_finalize_attack_and_flag_reactions(target->battle_data->battle_id);
                if (target->pending_attack_result == -1) {
                    battle_unit_find_relocation_tile(target->battle_data->battle_id, &target->dismount);
                }
            }
        }
    }

    if (g_action_phase == ACTION_PHASE_ACTING_UNIT) {
        if (unit->used_ability_id != 0 && !IS_ELEMENTAL_GUN(unit->used_item_or_weapon_id)
            && action->reaction_occurred == 0) {
            battle_unit_select_attack_animation_for_ability(unit, battle_unit_get_by_battle_id(action->target_list[0]));
        } else if (unit->used_ability_id == 0 || IS_ELEMENTAL_GUN(unit->used_item_or_weapon_id)) {
            if (action->target_count != 0) {
                battle_unit_select_weapon_attack_animation(unit, battle_unit_get_by_battle_id(action->target_list[0]));
            } else {
                battle_unit_select_weapon_attack_animation(unit, 0);
            }
        }
    } else if (unit->used_ability_id == ABILITY_ID_KNOCKBACK) {
        for (i = 0; i < unit->strike.target_count; i++) {
            target = battle_unit_get_by_battle_id(action->target_list[i]);
            if (target != 0) {
                target->map_x = unit->strike.target_new_x;
                target->map_y = unit->strike.target_new_y;
                target->map_z = unit->strike.target_new_map_level;
                battle_unit_set_tile_position(
                    target->battle_data->battle_id, target->map_x, target->map_y, target->map_z, target->facing / 1024);
                battle_unit_set_move_and_screen_coords(target);
            }
        }
    } else if (action->reaction_ability_id != ABILITY_ID_REACTION_REFLECT) {
        if (unit->used_ability_id != 0 && !IS_ELEMENTAL_GUN(unit->used_item_or_weapon_id)
            && unit->continue_attack_count == 0) {
            if (action->reaction_occurred == 0) {
                battle_unit_select_attack_animation_for_ability(
                    unit, battle_unit_get_by_battle_id(action->target_list[0]));
            }
        } else if (unit->used_ability_id == 0 || IS_ELEMENTAL_GUN(unit->used_item_or_weapon_id)) {
            if (action->target_count != 0) {
                battle_unit_select_weapon_attack_animation(unit, battle_unit_get_by_battle_id(action->target_list[0]));
            } else {
                battle_unit_select_weapon_attack_animation(unit, 0);
            }
        }
    }
}
