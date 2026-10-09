#include "fft/battle.h"
#include "psx/types.h"

/*
 * Applies this frame's pending status changes to a unit's renderer state:
 * plays the add/remove animations for the two low status bits, raises the
 * death smoke and movement sound for a newly added status 1-4 bit 2, and then
 * picks the follow-up animation and its countdown from whether the unit is
 * floating or flying.
 */
void battle_unit_update_animation_for_status_changes(battle_unit_misc_data_t* unit) {
    battle_stats_t* stats;

    if (unit->statuses_to_add_5_6 & BATTLE_MISC_STATUS_CHICKEN) {
        battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_IDLE, unit->facing, unit);
    } else if (unit->statuses_to_remove_5_6 & BATTLE_MISC_STATUS_CHICKEN) {
        battle_unit_set_animation_based_on_status(unit);
    }
    if (unit->statuses_to_add_5_6 & BATTLE_MISC_STATUS_FROG) {
        battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_IDLE, unit->facing, unit);
    } else if (unit->statuses_to_remove_5_6 & BATTLE_MISC_STATUS_FROG) {
        battle_unit_set_animation_based_on_status(unit);
    }
    if (unit->statuses_to_add_1_4 & BATTLE_MISC_STATUS_DEAD) {
        if ((u32)(g_battle_gfx_spritesheet_data[unit->spritesheet_id].seq_id - BATTLE_SEQ_CYOKO) < 2) {
            battle_effect_set_secondary_death_smoke(unit);
        }
        if (g_battle_game_state != BATTLE_GAME_STATE_EVENT) {
            stats = unit->battle_data;
            if (stats != 0) {
                if (stats->unit_flags & UNIT_FLAG_MALE) {
                    battle_sound_play_movement_sfx(unit, MAIN_SFX_DEATH_MALE);
                } else if (stats->unit_flags & UNIT_FLAG_FEMALE) {
                    battle_sound_play_movement_sfx(unit, MAIN_SFX_DEATH_FEMALE);
                } else if (stats->unit_flags & UNIT_FLAG_MONSTER) {
                    battle_sound_play_movement_sfx(unit, MAIN_SFX_DEATH_MONSTER);
                }
            }
        }
    }
    if (unit->statuses_to_add_5_6 & (BATTLE_MISC_STATUS_CRYSTAL | BATTLE_MISC_STATUS_TREASURE)) {
        battle_effect_set_secondary_zodiac_poof(unit);
    }
    if (g_battle_game_state == BATTLE_GAME_STATE_EVENT) {
        if (unit->statuses_to_add_5_6 & BATTLE_MISC_STATUS_JUMP) {
            battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_JUMP_DEPART, unit->facing, unit);
        } else if (unit->statuses_to_remove_5_6 & BATTLE_MISC_STATUS_JUMP) {
            battle_unit_draw_and_activate_by_misc_id(unit->unit_id);
            battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_JUMP_LAND, unit->facing, unit);
        }
        if ((u32)(battle_move_validate_float_fly(unit) & 0xff) >= 2) {
            if (unit->statuses_to_add_1_4 & BATTLE_MISC_STATUS_DEAD) {
                battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_DEAD, unit->facing, unit);
                unit->encoded_animation = BATTLE_UNIT_ANIMATION_DEAD << 1;
                unit->animation_countdown = 0;
            } else if (unit->statuses_to_remove_1_4 & BATTLE_MISC_STATUS_DEAD) {
                battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_FLOAT_IDLE, unit->facing, unit);
                unit->encoded_animation = BATTLE_UNIT_ANIMATION_FLOAT_IDLE << 1;
                unit->animation_countdown = 0;
            }
        }
    } else if ((u32)(battle_move_validate_float_fly(unit) & 0xff) < 2) {
        if (unit->statuses_to_add_1_4 & BATTLE_MISC_STATUS_DEAD) {
            battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_DEATH, unit->facing, unit);
        } else if (unit->statuses_to_remove_1_4 & BATTLE_MISC_STATUS_DEAD) {
            battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_REVIVE, unit->facing, unit);
        }
    } else if (unit->statuses_to_add_1_4 & BATTLE_MISC_STATUS_DEAD) {
        battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_DEAD, unit->facing, unit);
        unit->encoded_animation = BATTLE_UNIT_ANIMATION_DEAD << 1;
        unit->animation_countdown = 0;
    } else if (unit->statuses_to_remove_1_4 & BATTLE_MISC_STATUS_DEAD) {
        battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_FLOAT_IDLE, unit->facing, unit);
        unit->encoded_animation = BATTLE_UNIT_ANIMATION_FLOAT_IDLE << 1;
        unit->animation_countdown = 0;
    }
}
