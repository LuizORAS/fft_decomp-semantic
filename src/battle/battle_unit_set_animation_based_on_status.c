#include "fft/battle.h"
#include "psx/types.h"

/* Pick a unit's standing animation from its status mirror and start it with its facing. Out of
 * deep water (battle_move_get_water_animation_mode below 2): Crystal or Treasure first; with no
 * animation status the plain stance (6 on flying spritesheets, 3 otherwise); else the first of Dead,
 * Mounted and Critical (these two only on spritesheets with a SEQ id below 5, Critical not on a
 * mount), Stop, Sleep, Petrify, Confusion, Performing or Charging (the last ability's charge
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
    /* A combined 0..1 range test folds to one unsigned compare; the target
     * tests the sign first, so that path jumps into the submerged arm. */
    if (water_mode < 0) {
        goto submerged;
    }
    if (water_mode < 2) {
        if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_CRYSTAL) {
            animation = 9;
        } else if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_TREASURE) {
            animation = 0x15;
        } else if ((unit->status_flags_1_4 & BATTLE_MISC_STATUS_ANIMATION_SELECTION_MASK) == 0) {
            animation = 6;
            if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                animation = 3;
            }
        } else {
            spritesheet_id = unit->spritesheet_id;
            if (g_battle_gfx_spritesheet_data[spritesheet_id].seq_id >= 5) {
                if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_DEAD) {
                    animation = 0x1a;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_STOP) {
                    animation = 2;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_SLEEP) {
                    animation = 0x24;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_PETRIFY) {
                    animation = 2;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CONFUSION) {
                    animation = 0x25;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_PERFORMING) {
                    unit->used_ability_id = unit->battle_data->last_ability_id;
                    battle_unit_start_ability_charge_animation_for_movement(unit);
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CHARGING) {
                    unit->used_ability_id = unit->battle_data->last_ability_id;
                    battle_unit_start_ability_charge_animation_for_movement(unit);
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_DEFENDING) {
                    animation = 0x17;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_SLOW) {
                    animation = 8;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = 5;
                    }
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_HASTE) {
                    animation = 7;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = 4;
                    }
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CURSED) {
                    animation = 2;
                } else if (unit->status_flags_1_4 & 1) {
                    animation = 0x21;
                } else {
                    animation = 6;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = 3;
                    }
                }
            } else {
                if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_DEAD) {
                    animation = 0x1a;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_MOUNTED) {
                    animation = 0x32;
                } else if ((unit->status_flags_1_4 & BATTLE_MISC_STATUS_CRITICAL)
                    && unit->mount_state != BATTLE_MISC_MOUNT_STATE_MOUNT) {
                    animation = 0x24;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_STOP) {
                    animation = 2;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_SLEEP) {
                    animation = 0x24;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_PETRIFY) {
                    animation = 2;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CONFUSION) {
                    animation = 0x25;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_PERFORMING) {
                    unit->used_ability_id = unit->battle_data->last_ability_id;
                    battle_unit_start_ability_charge_animation_for_movement(unit);
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CHARGING) {
                    unit->used_ability_id = unit->battle_data->last_ability_id;
                    battle_unit_start_ability_charge_animation_for_movement(unit);
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_DEFENDING) {
                    animation = 0x17;
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_SLOW) {
                    animation = 8;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = 5;
                    }
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_HASTE) {
                    animation = 7;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = 4;
                    }
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CURSED) {
                    animation = 2;
                } else if (unit->status_flags_1_4 & 1) {
                    animation = 0x21;
                } else {
                    animation = 6;
                    if (battle_gfx_get_spritesheet_flying_flag(unit->spritesheet_id) == 0) {
                        animation = 3;
                    }
                }
            }
        }
    } else {
    submerged:
        if ((unit->status_flags_5_6 & BATTLE_MISC_STATUS_TRANSFORMATION_MASK) != 0) {
            if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_CRYSTAL) {
                animation = 9;
            } else if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_TREASURE) {
                animation = 0x15;
            }
        } else if ((unit->status_flags_1_4 & BATTLE_MISC_STATUS_ANIMATION_SELECTION_MASK) != 0) {
            if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_DEAD) {
                animation = 0x1a;
            } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_MOUNTED) {
                animation = 0x32;
            } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_HASTE) {
                animation = 0xa;
            } else {
                animation = 9;
                if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_SLOW) {
                    animation = 0xb;
                }
            }
        } else {
            animation = 9;
        }
    }
    if (animation != 0) {
        battle_unit_store_animation_facing(animation, unit->facing, unit);
    }
}
