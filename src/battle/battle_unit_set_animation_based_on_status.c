#include "fft/battle.h"
#include "psx/types.h"

/* Pick a unit's standing animation from its status mirror and start it with its facing. Out of
 * deep water (battle_move_get_water_animation_mode below 2): Crystal or Treasure first; with no
 * animation status the plain stance (6 on flying spritesheets, 3 otherwise); else the first of Dead,
 * Mounted and Critical (these two not for the Lucavi and Altima, BATTLE_SEQ_RUKA and up;
 * Critical not on a mount), Stop, Sleep, Petrify, Confusion, Performing or Charging (the last ability's charge
 * animation), Defending, Slow, Haste and Cursed, or the plain stance. In deep water: Crystal or Treasure, else Dead,
 * Mounted, Haste, Slow or the submerged stance (9).
 *
 * The negative water-mode test never passes (the mode is masked to a byte) but keeps the target's
 * branch. No status sets bit 0x01 of status_flags_1_4, so its animation 0x21 never plays
 * (QUIRKS.md). Animation indices are spritesheet-local and therefore remain literals. */
void battle_unit_set_animation_based_on_status(battle_unit_misc_data_t* unit) {
    s32 animation;
    s32 water_mode;
    u8 spritesheet_id;

    water_mode = battle_move_get_water_animation_mode(unit) & 0xff;
    animation = 0;
    switch (water_mode) {
    case 0:
    case 1:
        if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_CRYSTAL) {
            animation = BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE;
        } else if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_TREASURE) {
            animation = BATTLE_UNIT_ANIMATION_TREASURE;
        } else if ((unit->status_flags_1_4 & BATTLE_MISC_STATUS_ANIMATION_SELECTION_MASK) == 0) {
            animation = BATTLE_UNIT_ANIMATION_IDLE_FLYING;
            if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                animation = BATTLE_UNIT_ANIMATION_IDLE;
            }
        } else {
            spritesheet_id = unit->spritesheet_id;
            if (g_battle_gfx_spritesheet_data[spritesheet_id].seq_id >= BATTLE_SEQ_RUKA) {
                if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_DEAD) {
                    animation = BATTLE_UNIT_ANIMATION_DEAD;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_STOP) {
                    animation = BATTLE_UNIT_ANIMATION_STAND;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_SLEEP) {
                    animation = BATTLE_UNIT_ANIMATION_SLEEP;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_PETRIFY) {
                    animation = BATTLE_UNIT_ANIMATION_STAND;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CONFUSION) {
                    animation = BATTLE_UNIT_ANIMATION_CONFUSION;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_PERFORMING) {
                    unit->used_ability_id = unit->battle_data->last_ability_id;
                    battle_unit_start_ability_charging_animation(unit);
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CHARGING) {
                    unit->used_ability_id = unit->battle_data->last_ability_id;
                    battle_unit_start_ability_charging_animation(unit);
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_DEFENDING) {
                    animation = BATTLE_UNIT_ANIMATION_DEFEND;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_SLOW) {
                    animation = BATTLE_UNIT_ANIMATION_IDLE_FLYING_SLOW;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = BATTLE_UNIT_ANIMATION_IDLE_SLOW;
                    }
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_HASTE) {
                    animation = BATTLE_UNIT_ANIMATION_IDLE_FLYING_HASTE;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = BATTLE_UNIT_ANIMATION_IDLE_HASTE;
                    }
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_UNNAMED_0E) {
                    animation = BATTLE_UNIT_ANIMATION_STAND;
                } else if (unit->status_flags_1_4 & 1) {
                    animation = 0x21;
                } else {
                    animation = BATTLE_UNIT_ANIMATION_IDLE_FLYING;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = BATTLE_UNIT_ANIMATION_IDLE;
                    }
                }
            } else {
                if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_DEAD) {
                    animation = BATTLE_UNIT_ANIMATION_DEAD;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_MOUNTED) {
                    animation = BATTLE_UNIT_ANIMATION_RIDING;
                } else if ((unit->status_flags_1_4 & BATTLE_MISC_STATUS_CRITICAL)
                    && unit->mount_state != BATTLE_MISC_MOUNT_STATE_MOUNT) {
                    animation = BATTLE_UNIT_ANIMATION_SLEEP;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_STOP) {
                    animation = BATTLE_UNIT_ANIMATION_STAND;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_SLEEP) {
                    animation = BATTLE_UNIT_ANIMATION_SLEEP;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_PETRIFY) {
                    animation = BATTLE_UNIT_ANIMATION_STAND;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CONFUSION) {
                    animation = BATTLE_UNIT_ANIMATION_CONFUSION;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_PERFORMING) {
                    unit->used_ability_id = unit->battle_data->last_ability_id;
                    battle_unit_start_ability_charging_animation(unit);
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CHARGING) {
                    unit->used_ability_id = unit->battle_data->last_ability_id;
                    battle_unit_start_ability_charging_animation(unit);
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_DEFENDING) {
                    animation = BATTLE_UNIT_ANIMATION_DEFEND;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_SLOW) {
                    animation = BATTLE_UNIT_ANIMATION_IDLE_FLYING_SLOW;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = BATTLE_UNIT_ANIMATION_IDLE_SLOW;
                    }
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_HASTE) {
                    animation = BATTLE_UNIT_ANIMATION_IDLE_FLYING_HASTE;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = BATTLE_UNIT_ANIMATION_IDLE_HASTE;
                    }
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_UNNAMED_0E) {
                    animation = BATTLE_UNIT_ANIMATION_STAND;
                } else if (unit->status_flags_1_4 & 1) {
                    animation = 0x21;
                } else {
                    animation = BATTLE_UNIT_ANIMATION_IDLE_FLYING;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = BATTLE_UNIT_ANIMATION_IDLE;
                    }
                }
            }
        }
        break;
    case 2:
    default:
        if ((unit->status_flags_5_6 & BATTLE_MISC_STATUS_TRANSFORMATION_MASK) != 0) {
            if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_CRYSTAL) {
                animation = BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE;
            } else if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_TREASURE) {
                animation = BATTLE_UNIT_ANIMATION_TREASURE;
            }
        } else if ((unit->status_flags_1_4 & BATTLE_MISC_STATUS_ANIMATION_SELECTION_MASK) != 0) {
            if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_DEAD) {
                animation = BATTLE_UNIT_ANIMATION_DEAD;
            } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_MOUNTED) {
                animation = BATTLE_UNIT_ANIMATION_RIDING;
            } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_HASTE) {
                animation = BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE_HASTE;
            } else {
                animation = BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE;
                if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_SLOW) {
                    animation = BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE_SLOW;
                }
            }
        } else {
            animation = BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE;
        }
        break;
    }
    if (animation != 0) {
        battle_unit_store_animation_facing(animation, unit->facing, unit);
    }
}
