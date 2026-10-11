#include "fft/main.h"

/*
 * Fills an in-battle unit record from its persistent party record.
 */
void main_unit_init_battle_data(battle_stats_t* unit, const party_data_t* party) {
    s32 team;
    s32 i;
    s32 value;
    battle_unit_birthday_t* birthday;

    team = 8;
    if (party->sprite_set < CHARACTER_IDENTITY_RAMZA_END) {
        team = 0xb;
    }
    unit->team_flags = team;
    unit->initial_team_flags = team;
    unit->character_identity = party->sprite_set;
    unit->formation_index = party->party_id;
    unit->job_id = party->job_id;
    value = party->palette;
    if (party->party_id < PARTY_GUEST_SLOT_FIRST) {
        value = 0;
    }
    unit->sprite_palette = value;
    unit->unit_flags = party->gender_flags;
    value = party->birthday_day + (party->zodiac << 8);
    birthday = &unit->birthday;
    birthday->birthday = value;
    birthday->zodiac = party->zodiac >> PARTY_ZODIAC_SHIFT;
    unit->secondary_skillset = party->secondary_skillset;
    unit->reaction_ability = party->reaction_ability[0] + (party->reaction_ability[1] << 8);
    unit->support_ability = party->support_ability[0] + (party->support_ability[1] << 8);
    unit->movement_ability = party->movement_ability[0] + (party->movement_ability[1] << 8);
    main_util_copy_byte_data(party->equipment, unit->equipment, UNIT_EQUIPMENT_SLOT_COUNT);
    unit->experience = party->experience;
    unit->level = party->level;
    unit->original_brave = unit->brave = party->bravery;
    unit->original_faith = unit->faith = party->faith;
    main_util_copy_byte_data(&party->raw_stats, &unit->raw_stats, sizeof(unit_raw_stats_t));
    for (i = 0; i < UNIT_UNLOCKED_JOB_BYTE_COUNT; i++) {
        unit->career.unlocked_jobs[i] = party->career.unlocked_jobs[i];
    }
    main_util_copy_byte_data(
        party->career.learned_abilities, unit->career.learned_abilities, UNIT_LEARNED_ABILITY_BYTE_COUNT);
    main_util_copy_byte_data(party->career.job_levels, unit->career.job_levels, UNIT_JOB_LEVEL_BYTE_COUNT);
    /* Copy the adjacent current/total JP tables as their 80 serialized bytes. */
    main_util_copy_byte_data(&party->career.job_points, &unit->career.job_points, sizeof(unit->career.job_points));
    main_util_copy_byte_data(party->name, unit->name, sizeof(unit->name));
    unit->quote_name_id = party->name_id[0] + (party->name_id[1] << 8);
    main_util_clear_byte_data(&unit->ai_target_x, 7);
}
