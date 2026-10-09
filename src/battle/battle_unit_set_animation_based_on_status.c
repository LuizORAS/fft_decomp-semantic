#include "fft/battle.h"
#include "psx/types.h"

/*
 * Selects the unit animation from its renderer-side status mirror.
 *
 * Animation 0x21 belongs to status bit 0, which has no name.
 */
void battle_unit_set_animation_based_on_status(battle_unit_misc_data_t* unit) {
    s32 animation;
    s32 float_fly;
    u8 spritesheet_id;

    float_fly = battle_move_validate_float_fly(unit) & 0xff;
    animation = 0;
    switch (float_fly) {
    case 0:
    case 1:
        if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_CRYSTAL) {
            animation = BATTLE_UNIT_ANIMATION_FLOAT_IDLE;
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
                    battle_unit_start_ability_charge_hold_animation(unit);
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CHARGING) {
                    unit->used_ability_id = unit->battle_data->last_ability_id;
                    battle_unit_start_ability_charge_hold_animation(unit);
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
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CURSED) {
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
                    battle_unit_start_ability_charge_hold_animation(unit);
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CHARGING) {
                    unit->used_ability_id = unit->battle_data->last_ability_id;
                    battle_unit_start_ability_charge_hold_animation(unit);
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
                } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_CURSED) {
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
                animation = BATTLE_UNIT_ANIMATION_FLOAT_IDLE;
            } else if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_TREASURE) {
                animation = BATTLE_UNIT_ANIMATION_TREASURE;
            }
        } else if ((unit->status_flags_1_4 & BATTLE_MISC_STATUS_ANIMATION_SELECTION_MASK) != 0) {
            if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_DEAD) {
                animation = BATTLE_UNIT_ANIMATION_DEAD;
            } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_MOUNTED) {
                animation = BATTLE_UNIT_ANIMATION_RIDING;
            } else if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_HASTE) {
                animation = BATTLE_UNIT_ANIMATION_FLOAT_IDLE_HASTE;
            } else {
                animation = BATTLE_UNIT_ANIMATION_FLOAT_IDLE;
                if (unit->status_flags_1_4 & BATTLE_MISC_STATUS_SLOW) {
                    animation = BATTLE_UNIT_ANIMATION_FLOAT_IDLE_SLOW;
                }
            }
        } else {
            animation = BATTLE_UNIT_ANIMATION_FLOAT_IDLE;
        }
        break;
    }
    if (animation != 0) {
        battle_unit_store_animation_facing(animation, unit->facing, unit);
    }
}
