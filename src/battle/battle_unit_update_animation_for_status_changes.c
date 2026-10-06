#include "fft/battle.h"
#include "psx/types.h"

/* Play this frame's status graphics changes on a unit (battle_unit_update_display calls it).
 * Becoming a Chicken or Frog starts animation 3; losing either picks the status animation again
 * (battle_unit_set_animation_based_on_status). Dying raises the death smoke (spritesheets with SEQ id
 * 2 or 3) and, outside events, the death sound of a male, female or monster (0x9a, 0x45, 0x46).
 * Becoming a Crystal or Treasure shows the zodiac poof. During an event, gaining or losing Jump plays
 * animation 0x75 or 0x76. Then a death or revival plays its fall or rise: 0x34 or 0x35 on land, the
 * submerged pair (0x1a, 9) in deep water; during an event only in deep water. */
void battle_unit_update_animation_for_status_changes(battle_unit_misc_data_t* unit) {
    battle_stats_t* stats;

    if (unit->statuses_to_add_5_6 & BATTLE_MISC_STATUS_CHICKEN) {
        battle_unit_store_animation_facing_movement_data(3, unit->facing, unit);
    } else if (unit->statuses_to_remove_5_6 & BATTLE_MISC_STATUS_CHICKEN) {
        battle_unit_set_animation_based_on_status(unit);
    }
    if (unit->statuses_to_add_5_6 & BATTLE_MISC_STATUS_FROG) {
        battle_unit_store_animation_facing_movement_data(3, unit->facing, unit);
    } else if (unit->statuses_to_remove_5_6 & BATTLE_MISC_STATUS_FROG) {
        battle_unit_set_animation_based_on_status(unit);
    }
    if (unit->statuses_to_add_1_4 & BATTLE_MISC_STATUS_DEAD) {
        if ((u32)(g_battle_gfx_spritesheet_data[unit->spritesheet_id].seq_id - 2) < 2) {
            battle_effect_set_secondary_death_smoke(unit);
        }
        if (g_battle_game_state != BATTLE_GAME_STATE_EVENT) {
            stats = unit->battle_data;
            if (stats != 0) {
                if (stats->unit_flags & UNIT_FLAG_MALE) {
                    battle_sound_play_movement_sfx(unit, 0x9a);
                } else if (stats->unit_flags & UNIT_FLAG_FEMALE) {
                    battle_sound_play_movement_sfx(unit, 0x45);
                } else if (stats->unit_flags & UNIT_FLAG_MONSTER) {
                    battle_sound_play_movement_sfx(unit, 0x46);
                }
            }
        }
    }
    if (unit->statuses_to_add_5_6 & (BATTLE_MISC_STATUS_CRYSTAL | BATTLE_MISC_STATUS_TREASURE)) {
        battle_effect_set_secondary_zodiac_poof(unit);
    }
    if (g_battle_game_state == BATTLE_GAME_STATE_EVENT) {
        if (unit->statuses_to_add_5_6 & BATTLE_MISC_STATUS_JUMP) {
            battle_unit_store_animation_facing_movement_data(0x75, unit->facing, unit);
        } else if (unit->statuses_to_remove_5_6 & BATTLE_MISC_STATUS_JUMP) {
            battle_unit_animate_and_set_enemy_level_data_by_misc_id(unit->unit_id);
            battle_unit_store_animation_facing_movement_data(0x76, unit->facing, unit);
        }
        if ((u32)(battle_move_get_water_animation_mode(unit) & 0xff) >= 2) {
            if (unit->statuses_to_add_1_4 & BATTLE_MISC_STATUS_DEAD) {
                battle_unit_store_animation_facing_movement_data(0x1a, unit->facing, unit);
                unit->encoded_animation = 0x34;
                unit->animation_countdown = 0;
            } else if (unit->statuses_to_remove_1_4 & BATTLE_MISC_STATUS_DEAD) {
                battle_unit_store_animation_facing_movement_data(9, unit->facing, unit);
                unit->encoded_animation = 0x12;
                unit->animation_countdown = 0;
            }
        }
    } else if ((u32)(battle_move_get_water_animation_mode(unit) & 0xff) < 2) {
        if (unit->statuses_to_add_1_4 & BATTLE_MISC_STATUS_DEAD) {
            battle_unit_store_animation_facing(0x34, unit->facing, unit);
        } else if (unit->statuses_to_remove_1_4 & BATTLE_MISC_STATUS_DEAD) {
            battle_unit_store_animation_facing(0x35, unit->facing, unit);
        }
    } else if (unit->statuses_to_add_1_4 & BATTLE_MISC_STATUS_DEAD) {
        battle_unit_store_animation_facing_movement_data(0x1a, unit->facing, unit);
        unit->encoded_animation = 0x34;
        unit->animation_countdown = 0;
    } else if (unit->statuses_to_remove_1_4 & BATTLE_MISC_STATUS_DEAD) {
        battle_unit_store_animation_facing_movement_data(9, unit->facing, unit);
        unit->encoded_animation = 0x12;
        unit->animation_countdown = 0;
    }
}
