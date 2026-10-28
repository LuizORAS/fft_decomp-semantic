#include "fft/main.h"
#include "fft/memory.h"
#include "fft/unit.h"

/* Apply the equipped items' attribute records to a unit. The stats are derived again first
 * (main_unit_calculate_actual_stats; with reset_base_attributes also base Speed, PA and MA); then
 * each item adds its PA, MA and Speed bonuses (cleared first, totals at most 255), Move (at most
 * 253), Jump (at most 7), its innate, immunity and starting statuses, its elemental affinities and,
 * for headgear and body armor, its HP and MP bonuses (at most 999); the equipped flags note the
 * Materia Blade and any sword or knight sword. A monster takes no equipment. HP and MP are cut to the
 * new maxima, and the actual PA and MA (at most 99) and Speed (at most 50) are base plus bonus. */
void main_unit_set_equipment_attributes(battle_stats_t* unit, s32 reset_base_attributes) {
    s32 i;
    s32 j;
    u32 value;
    u32 cap;
    u8 item_id;
    u32 second_id;
    item_data_t* item;
    item_attribute_t* attribute;
    u8* bonus; /* equipment bonuses, later reused to walk the base/actual stats */
    u8* source;
    u8* target;

    main_mem_zero(unit->equipment_attributes, sizeof(unit->equipment_attributes));
    cap = MAX_HP_MP_CAP;
    main_unit_calculate_actual_stats(unit, reset_base_attributes + 1);
    unit->equipped_flags = 0;
    if ((unit->unit_flags & UNIT_FLAG_MONSTER) == 0) {
        bonus = unit->equipment_attributes;
        for (i = 0; i < UNIT_EQUIPMENT_SLOT_COUNT; i++) {
            item_id = unit->equipment[i];
            if (item_id == ITEM_ID_MATERIA_BLADE) {
                unit->equipped_flags |= BATTLE_UNIT_EQUIPPED_FLAG_MATERIA_BLADE;
            }
            if (item_id < ITEM_ID_SHIELD_FIRST) {
                if (g_main_item_primary_data[item_id].type == ITEM_TYPE_SWORD
                    || g_main_item_primary_data[item_id].type == ITEM_TYPE_KNIGHT_SWORD) {
                    unit->equipped_flags |= BATTLE_UNIT_EQUIPPED_FLAG_SWORD;
                }
            }
            if (item_id == ITEM_ID_NONE) {
                continue;
            }
            item = &g_main_item_primary_data[item_id];
            attribute = &g_main_item_attributes[item->attributes];
            source = attribute->attributes;
            for (j = 0; j < 3; j++) {
                value = source[j] + bonus[j];
                if (value >= 0x100) {
                    value = 0xff;
                }
                bonus[j] = value;
            }
            value = attribute->move + unit->move;
            if (value >= 0xfe) {
                value = 0xfd;
            }
            unit->move = value;
            value = attribute->jump + unit->jump;
            if (value >= 8) {
                value = 7;
            }
            unit->jump = value;
            j = 0;
            target = unit->status_sets.innate;
            for (; j < 15; j++) {
                *target = attribute->status_sets.innate[j] | *target;
                target++;
            }
            j = 0;
            target = unit->elemental_affinity;
            for (; j < ELEMENTAL_AFFINITY_COUNT; j++) {
                *target = attribute->elemental_affinity[j] | *target;
                target++;
            }
            if (item->type_flags & (ITEM_TYPE_FLAG_HEADGEAR | ITEM_TYPE_FLAG_BODY_ARMOR)) {
                second_id = item->secondary_data_id;
                value = unit->max_hp + g_main_item_helm_armor_secondary_data[second_id].hp_bonus;
                if (value > cap) {
                    value = cap;
                }
                unit->max_hp = value;
                value = unit->max_mp + g_main_item_helm_armor_secondary_data[second_id].mp_bonus;
                if (value > cap) {
                    /* The original routes the cap through the loop scratch; a plain
                     * `value = cap` changes the register allocation. */
                    value = j = cap;
                }
                unit->max_mp = value;
            }
        }
    }
    if (unit->hp > unit->max_hp) {
        unit->hp = unit->max_hp;
    }
    if (unit->mp > unit->max_mp) {
        unit->mp = unit->max_mp;
    }
    bonus = unit->base_attributes;
    for (j = 0; j < 3; j++) {
        value = bonus[j + 3] + bonus[j];
        if (j < 2) {
            if (value > UNIT_ATTACK_STAT_MAX) {
                value = UNIT_ATTACK_STAT_MAX;
            }
        } else {
            if ((u16)value > UNIT_SPEED_MAX) {
                value = UNIT_SPEED_MAX;
            }
        }
        bonus[j + 6] = value;
    }
}
