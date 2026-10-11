#include "fft/battle.h"
#include "psx/types.h"

/* Play this frame's status graphics changes on a unit (battle_unit_update_display calls it).
 * Becoming a Chicken or Frog starts the idle animation; losing either picks the status animation
 * again (battle_unit_set_animation_based_on_status). Dying raises the death smoke (chocobo and
 * monster SEQs) and, outside events, the death cry of a male, female or monster unit. Becoming a
 * Crystal or Treasure shows the zodiac poof. During an event, gaining or losing Jump plays the
 * jump's departure or landing. Then a death or revival plays its fall or rise (DEATH, REVIVE) on
 * land; where the unit is submerged (water animation mode 2 or more) it goes straight into its dead
 * or submerged idle sequence, with the wait cleared so the sequence starts at once. During an event
 * only the submerged case applies. */
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
        if (g_battle_gfx_spritesheet_data[unit->spritesheet_id].seq_id == BATTLE_SEQ_CYOKO
            || g_battle_gfx_spritesheet_data[unit->spritesheet_id].seq_id == BATTLE_SEQ_MON) {
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
        if ((battle_move_get_water_animation_mode(unit) & 0xff) >= 2) {
            if (unit->statuses_to_add_1_4 & BATTLE_MISC_STATUS_DEAD) {
                battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_DEAD, unit->facing, unit);
                unit->sequence_id = BATTLE_UNIT_ANIMATION_DEAD << 1;
                unit->animation_wait = 0;
            } else if (unit->statuses_to_remove_1_4 & BATTLE_MISC_STATUS_DEAD) {
                battle_unit_store_animation_facing_movement_data(
                    BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE, unit->facing, unit);
                unit->sequence_id = BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE << 1;
                unit->animation_wait = 0;
            }
        }
    } else if ((battle_move_get_water_animation_mode(unit) & 0xff) < 2) {
        if (unit->statuses_to_add_1_4 & BATTLE_MISC_STATUS_DEAD) {
            battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_DEATH, unit->facing, unit);
        } else if (unit->statuses_to_remove_1_4 & BATTLE_MISC_STATUS_DEAD) {
            battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_REVIVE, unit->facing, unit);
        }
    } else if (unit->statuses_to_add_1_4 & BATTLE_MISC_STATUS_DEAD) {
        battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_DEAD, unit->facing, unit);
        unit->sequence_id = BATTLE_UNIT_ANIMATION_DEAD << 1;
        unit->animation_wait = 0;
    } else if (unit->statuses_to_remove_1_4 & BATTLE_MISC_STATUS_DEAD) {
        battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE, unit->facing, unit);
        unit->sequence_id = BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE << 1;
        unit->animation_wait = 0;
    }
}
