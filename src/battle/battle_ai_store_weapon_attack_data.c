#include "fft/battle.h"

/* Merge one hand's weapon into the unit's AI weapon summary: the range and weapon id are replaced (the
 * left hand, stored last, wins), the element accumulates, an arc or direct weapon sets that trajectory
 * flag, and the weapon's kind (BATTLE_AI_WEAPON_KIND_ABSORB for formula 6, _HEAL for 7, else _DAMAGE) is
 * ORed into *flags_out. */
void battle_ai_store_weapon_attack_data(battle_ai_weapon_data_t* out, s32 item_id, s32* flags_out) {
    weapon_data_t* weapon;
    s32 flags;
    s32 formula;

    if (item_id == ITEM_ID_NONE) {
        return;
    }
    weapon = &g_main_item_weapon_data[item_id];
    flags = weapon->flags;
    if (flags & WEAPON_FLAG_ARC) {
        out->bytes.flags_3 |= BATTLE_AI_ABILITY_FLAG_3_ARC_TRAJECTORY;
    } else if (flags & WEAPON_FLAG_DIRECT) {
        out->bytes.flags_3 |= BATTLE_AI_ABILITY_FLAG_3_DIRECT_TRAJECTORY;
    }
    formula = weapon->formula;
    switch (formula) {
    case BATTLE_FORMULA_WEAPON_ABSORB_HP:
        *flags_out |= BATTLE_AI_WEAPON_KIND_ABSORB;
        break;
    case BATTLE_FORMULA_WEAPON_HEAL:
        *flags_out |= BATTLE_AI_WEAPON_KIND_HEAL;
        break;
    default:
        *flags_out |= BATTLE_AI_WEAPON_KIND_DAMAGE;
        break;
    }
    out->bytes.weapon_id = (u8)item_id;
    out->bytes.range = weapon->range;
    out->bytes.element |= weapon->element;
}
