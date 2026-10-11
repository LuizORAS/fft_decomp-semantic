#include "fft/world.h"
#include "psx/types.h"

/*
 * Fills one WORLD formation record from a unit's battle stats and party data.
 *
 * HP/MP/level/experience/CT are clamped for display, monster-skillset units
 * (primary skillset 0xb0-0xdf) list up to three nonzero skillset abilities
 * instead of equipment and learned abilities, and JP totals cap at 9999.
 * One scratch variable `i` also carries the monster flag, the second evade
 * operand and the unlocked-job mask; separate locals reallocate a0/v1.
 */
void world_formation_build_unit_record(battle_stats_t* unit, world_formation_unit_t* record, party_data_t* party) {
    s16* abilities;
    s32 i;
    u32 evade;

    memset(record, 0, sizeof(world_formation_unit_t));
    record->level = party->level <= UNIT_LEVEL_MAX ? party->level : UNIT_LEVEL_MAX;
    if (unit->formation_index >= PARTY_GUEST_SLOT_FIRST) {
        record->team_kind = 2;
    }
    record->experience = party->experience < UNIT_LEVEL_UP_EXPERIENCE ? party->experience : UNIT_LEVEL_EXPERIENCE_MAX;
    record->party_index = unit->formation_index;
    record->hp = unit->max_hp <= UNIT_HP_MP_MAX ? unit->max_hp : UNIT_HP_MP_MAX;
    record->max_hp = unit->max_hp <= UNIT_HP_MP_MAX ? unit->max_hp : UNIT_HP_MP_MAX;
    record->mp = unit->max_mp <= UNIT_HP_MP_MAX ? unit->max_mp : UNIT_HP_MP_MAX;
    record->max_mp = unit->max_mp <= UNIT_HP_MP_MAX ? unit->max_mp : UNIT_HP_MP_MAX;
    record->ct = unit->ct <= UNIT_TURN_CT_THRESHOLD ? unit->ct : UNIT_TURN_CT_THRESHOLD;
    record->max_ct = UNIT_TURN_CT_THRESHOLD;
    record->identity.unit_index = unit->formation_index;
    record->identity.job_id = party->job_id;
    record->identity.brave = unit->brave;
    record->identity.faith = unit->faith;
    record->identity.zodiac = unit->birthday.zodiac;
    record->identity.roster_slot = unit->formation_index;
    record->move = unit->move;
    record->speed = unit->attributes[UNIT_ATTRIBUTE_SPEED];
    record->jump = unit->jump;
    record->right_weapon_power = unit->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_RIGHT_WEAPON_POWER];
    record->left_weapon_power = unit->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_LEFT_WEAPON_POWER];
    record->right_weapon_evade = unit->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_RIGHT_WEAPON_EVADE];
    record->left_weapon_evade = unit->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_LEFT_WEAPON_EVADE];
    i = unit->primary_skillset >= SKILLSET_ID_MONSTER_FIRST && unit->primary_skillset < SKILLSET_ID_END;
    record->uses_monster_skillset = i;
    record->physical_attack = unit->attributes[UNIT_ATTRIBUTE_PHYSICAL_ATTACK];
    record->physical_class_evade = unit->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_CLASS_PHYSICAL_EVADE];
    evade = unit->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_RIGHT_SHIELD_PHYSICAL_EVADE];
    i = unit->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_LEFT_SHIELD_PHYSICAL_EVADE];
    if (evade < i) {
        evade = i;
    }
    record->physical_shield_evade = evade;
    record->physical_accessory_evade = unit->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_ACCESSORY_PHYSICAL_EVADE];
    record->magical_attack = unit->attributes[UNIT_ATTRIBUTE_MAGIC_ATTACK];
    record->magical_class_evade = 0;
    evade = unit->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_RIGHT_SHIELD_MAGIC_EVADE];
    i = unit->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_LEFT_SHIELD_MAGIC_EVADE];
    if (evade < i) {
        evade = i;
    }
    record->magical_shield_evade = evade;
    record->magical_accessory_evade = unit->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_ACCESSORY_MAGIC_EVADE];

    if (record->uses_monster_skillset != 0) {
        s32 count;

        abilities = (s16*)main_ability_store_skillset_abilities(unit->primary_skillset, SKILLSET_ABILITY_FILTER_ACTION);
        count = 0;
        for (i = 0; i < 3; i++) {
            if (abilities[i] != 0) {
                record->abilities.slots[count++] = abilities[i];
            }
        }
    } else {
        record->abilities.fields.primary_skillset = unit->primary_skillset;
        if (record->identity.job_id != JOB_ID_MIME) {
            record->equipment[WORLD_EQUIPMENT_SLOT_RIGHT_HAND]
                = party->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON] != ITEM_ID_NONE
                ? party->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON]
                : party->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_SHIELD];
            record->equipment[WORLD_EQUIPMENT_SLOT_LEFT_HAND]
                = party->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON] != ITEM_ID_NONE
                ? party->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON]
                : party->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_SHIELD];
            record->equipment[WORLD_EQUIPMENT_SLOT_HEAD] = party->equipment[UNIT_EQUIPMENT_SLOT_HEAD];
            record->equipment[WORLD_EQUIPMENT_SLOT_BODY] = party->equipment[UNIT_EQUIPMENT_SLOT_BODY];
            record->equipment[WORLD_EQUIPMENT_SLOT_ACCESSORY] = party->equipment[UNIT_EQUIPMENT_SLOT_ACCESSORY];
            for (i = 0; i < WORLD_EQUIPMENT_SLOT_COUNT; i++) {
                if (record->equipment[i] >= ITEM_ID_END) {
                    record->equipment[i] = ITEM_ID_NOTHING;
                }
            }
            record->abilities.fields.secondary_skillset = party->secondary_skillset;
            record->abilities.fields.reaction_ability = party->reaction_ability[0] + (party->reaction_ability[1] << 8);
            record->abilities.fields.support_ability = party->support_ability[0] + (party->support_ability[1] << 8);
            record->abilities.fields.movement_ability = party->movement_ability[0] + (party->movement_ability[1] << 8);
        }
    }

    if (world_job_is_special_monster(unit->job_id) != 0 || record->uses_monster_skillset != 0) {
        bcopy(unit->career.unlocked_jobs, record->unlocked_jobs, sizeof(record->unlocked_jobs));
    } else {
        i = main_job_calculate_unlocked(
            unit->career.job_levels, unit->unit_flags & (UNIT_FLAG_FEMALE | UNIT_FLAG_MALE));
        record->unlocked_jobs[0] = i >> 16;
        record->unlocked_jobs[1] = i >> 8;
        record->unlocked_jobs[2] = i;
    }
    record->gender_flags = party->gender_flags;
    record->sprite_set = party->sprite_set;
    bcopy(unit->equipment_categories, record->equippable_item_types, sizeof(record->equippable_item_types));
    bcopy(party->career.learned_abilities, record->learned_abilities, sizeof(record->learned_abilities));
    bcopy(party->career.job_levels, record->job_levels, sizeof(record->job_levels));
    bcopy(&party->career.job_points.current, &record->job_points.current, sizeof(record->job_points.current));
    bcopy(&party->career.job_points.total, &record->job_points.total, sizeof(record->job_points.total));
    for (i = 0; i < JOB_ID_GENERIC_COUNT; i++) {
        if (record->job_points.current.values[i] >= 10000) {
            record->job_points.current.values[i] = 9999;
        }
        if (record->job_points.total.values[i] >= 10000) {
            record->job_points.total.values[i] = 9999;
        }
    }
    bcopy(party->name, record->name, sizeof(record->name));
    bcopy(unit->support_abilities, record->support_abilities, sizeof(record->support_abilities));
    record->proposition_status = party->proposition_status;
    record->monster_graphic = unit->monster_graphic;
    record->birthday = unit->birthday.birthday;
    record->egg_color = party->egg_color;
    record->two_handed = world_unit_is_two_handing_weapon(
        (s16*)record->equipment, record->support_abilities[2] & BATTLE_SUPPORT_SET_3_TWO_HANDS);
}
