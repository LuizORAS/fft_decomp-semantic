#include "fft/battle.h"
#include "fft/main.h"
#include "psx/types.h"

/* Point *out_ability_data at the ability's 8-byte record (g_main_ability_data: JP cost, learn rate,
 * type flags, AI flags) and *out_secondary_data at its class's own data, and return the class
 * (ability_type_e) from the id range: default abilities below 0x170 (the 14-byte range data), item
 * abilities (the item id), Throw (the thrown item type), Jump and Charge (two bytes each), Math
 * Skill, reaction, support and movement abilities (one byte each). */
s32 main_ability_calculate_pointers_and_type(s32 ability_id, u8** out_ability_data, u8** out_secondary_data) {
    ability_id &= ABILITY_ID_MASK;
    /* 0x8005ebf0: Ability Data 1, 8 bytes each. */
    *out_ability_data = (u8*)&g_main_ability_data[ability_id];
    if (ability_id < ABILITY_ID_ITEM_FIRST) {
        *out_secondary_data = (u8*)&g_main_ability_range_data[ability_id];
        return ABILITY_TYPE_DEFAULT;
    }
    if (ability_id < ABILITY_ID_THROW_FIRST) {
        *out_secondary_data = &g_main_item_ability_item_ids_by_ability_id[ability_id];
        return ABILITY_TYPE_ITEM;
    }
    if (ability_id < ABILITY_ID_JUMP_FIRST) {
        *out_secondary_data = &g_main_throw_ability_item_types_by_ability_id[ability_id];
        return ABILITY_TYPE_THROW;
    }
    if (ability_id < ABILITY_ID_CHARGE_FIRST) {
        *out_secondary_data = &g_main_jump_charge_ability_data_by_ability_id[ability_id * 2];
        return ABILITY_TYPE_JUMP;
    }
    if (ability_id < ABILITY_ID_MATH_FIRST) {
        *out_secondary_data = &g_main_jump_charge_ability_data_by_ability_id[ability_id * 2];
        return ABILITY_TYPE_CHARGE;
    }
    if (ability_id < ABILITY_ID_REACTION_FIRST) {
        *out_secondary_data = &g_main_math_rsm_ability_data_by_ability_id[ability_id];
        return ABILITY_TYPE_MATH;
    }
    if (ability_id < ABILITY_ID_SUPPORT_FIRST) {
        *out_secondary_data = &g_main_math_rsm_ability_data_by_ability_id[ability_id];
        return ABILITY_TYPE_REACTION;
    }
    if (ability_id < ABILITY_ID_MOVEMENT_FIRST) {
        *out_secondary_data = &g_main_math_rsm_ability_data_by_ability_id[ability_id];
        return ABILITY_TYPE_SUPPORT;
    }
    *out_secondary_data = &g_main_math_rsm_ability_data_by_ability_id[ability_id];
    return ABILITY_TYPE_MOVEMENT;
}
