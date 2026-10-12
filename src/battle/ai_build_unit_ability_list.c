#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/* Byte-oriented view of the same four-byte ability entry. */
typedef struct ai_ability_entry_byte_view {
    u8 ability_id_low;
    u8 unit_and_id_high;
    u8 skillset;
    u8 usage_flags;
} ai_ability_entry_byte_view_t;
typedef char ai_ability_entry_byte_view_must_be_4[sizeof(ai_ability_entry_byte_view_t) == 4 ? 1 : -1];

/*
 * Build a unit's ability list and weapon-attack properties.
 *
 * Non-monster skillsets include Attack and a secondary skillset. Frog Attack
 * is appended for every unit; a terminator is written only if space remains.
 */
void ai_build_unit_ability_list(s32 unit_id) {
    battle_stats_t* unit = &g_battle_unit_stats[unit_id];
    ai_data_t* ai = &g_ai_data_base;
    ai_weapon_data_t* weapon;
    s32 list_id = ai->unit_compact_indices[unit_id];
    s32 count = 0;
    s32 weapon_flags;

    if (unit->primary_skillset < SKILLSET_ID_MONSTER_FIRST) {
        ai->ability_lists[list_id][0].skillset = SKILLSET_ID_ATTACK;
        ai->ability_lists[list_id][0].packed_id = unit_id << AI_ABILITY_ENTRY_UNIT_SHIFT;
        weapon = &ai->unit_weapon_data[list_id];
        weapon->words[0] = 0;
        weapon->words[1] = 0;
        weapon->fields.flags_1 |= AI_ABILITY_FLAG_1_AFFECTS_HP;
        weapon->fields.flags_2 |= AI_ABILITY_FLAG_2_EVADEABLE;
        weapon->fields.flags_3 |= AI_ABILITY_FLAG_3_PHYSICAL_ATTACK;
        weapon_flags = AI_WEAPON_FORMULA_NONE;
        if (!(unit->unit_flags & UNIT_FLAG_MONSTER)) {
            ai_store_weapon_attack_data(weapon, unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON], &weapon_flags);
            ai_store_weapon_attack_data(weapon, unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON], &weapon_flags);
        }
        switch (weapon_flags) {
        case AI_WEAPON_FORMULA_NONE:
            weapon->fields.range = 1;
            /* Fall through to normal enemy targeting. */
        case AI_WEAPON_FORMULA_DAMAGE:
            weapon->fields.flags_1 |= AI_ABILITY_FLAG_1_TARGET_ENEMIES;
            break;
        case AI_WEAPON_FORMULA_ABSORB_HP:
            weapon->fields.flags_1 |= AI_ABILITY_FLAG_1_TARGET_ENEMIES;
            weapon->fields.flags_2 |= AI_ABILITY_FLAG_2_UNDEAD_REVERSE;
            break;
        case AI_WEAPON_FORMULA_HEAL:
            weapon->fields.flags_1 |= AI_ABILITY_FLAG_1_TARGET_ALLIES;
            weapon->fields.flags_2 |= AI_ABILITY_FLAG_2_UNDEAD_REVERSE;
            break;
        }
        count++;
    }
    count = ai_add_usable_skillset_abilities(unit_id, unit->primary_skillset, count);
    if (unit->primary_skillset < SKILLSET_ID_MONSTER_FIRST) {
        count = ai_add_usable_skillset_abilities(unit_id, unit->secondary_skillset, count);
    }
    ai->ability_lists[list_id][count].skillset = SKILLSET_ID_FROG_ATTACK;
    ai->ability_lists[list_id][count].packed_id = (unit_id << AI_ABILITY_ENTRY_UNIT_SHIFT) + ABILITY_ID_FROG_ATTACK;
    /* The byte view preserves the target's separate final-store address. */
    ((ai_ability_entry_byte_view_t*)&ai->ability_lists[list_id][count])->usage_flags
        = AI_ABILITY_ENTRY_USABLE | AI_ABILITY_ENTRY_EXHAUSTIVE_ORIGIN_SCAN;
    count++;
    if (count < AI_ABILITY_LIST_COUNT) {
        ai->ability_lists[list_id][count].skillset = SKILLSET_ID_NONE;
    }
}
