#include "fft/battle_ai.h"

/*
 * Merge one weapon's attack properties into the unit's AI weapon summary.
 *
 * Range and weapon ID are replaced; elements and trajectory/formula flags
 * accumulate. Calling for the left hand last gives its range precedence.
 */
void ai_store_weapon_attack_data(ai_weapon_data_t* out, s32 item_id, s32* flags_out) {
    weapon_data_t* weapon;
    s32 flags;
    s32 formula;

    if (item_id == ITEM_ID_NONE) {
        return;
    }
    weapon = &g_main_item_weapon_data[item_id];
    flags = weapon->flags;
    if (flags & WEAPON_FLAG_ARC) {
        out->bytes.flags_3 |= AI_ABILITY_FLAG_3_ARC_TRAJECTORY;
    } else if (flags & WEAPON_FLAG_DIRECT) {
        out->bytes.flags_3 |= AI_ABILITY_FLAG_3_DIRECT_TRAJECTORY;
    }
    formula = weapon->formula;
    switch (formula) {
    case ABILITY_FORMULA_WEAPON_ABSORB_HP:
        *flags_out |= AI_WEAPON_FORMULA_ABSORB_HP;
        break;
    case ABILITY_FORMULA_WEAPON_HEAL:
        *flags_out |= AI_WEAPON_FORMULA_HEAL;
        break;
    default:
        *flags_out |= AI_WEAPON_FORMULA_DAMAGE;
        break;
    }
    out->bytes.weapon_id = item_id;
    out->bytes.range = weapon->range;
    out->bytes.element |= weapon->element;
}
