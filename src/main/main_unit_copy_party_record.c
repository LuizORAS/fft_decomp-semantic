#include "fft/main.h"

/* Copy a party record into a battle unit: identity, party slot, job, sex flags, birthday and zodiac,
 * secondary skillset, R/S/M abilities, equipment, EXP, level, Brave and Faith (with their originals),
 * raw stats, unlocked jobs, learned abilities, job levels, JP and the name. The unit joins the
 * player's team, controlled, Ramza at the top importance; a roster unit takes palette 0 and a guest
 * its own. The ENTD AI bytes are cleared. */
void main_unit_copy_party_record(battle_stats_t* unit, const party_data_t* party) {
    s32 team;
    s32 i;
    s32 value;
    battle_unit_birthday_t* birthday;

    team = BATTLE_TEAM_BLUE | BATTLE_TEAM_FLAG_PLAYER_CONTROLLED;
    if (party->sprite_set < CHARACTER_IDENTITY_RAMZA_END) {
        team = BATTLE_TEAM_BLUE | BATTLE_TEAM_FLAG_PLAYER_CONTROLLED | BATTLE_TEAM_IMPORTANCE_MASK;
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
    birthday->fields.birthday = value;
    birthday->fields.zodiac = party->zodiac >> PARTY_ZODIAC_SHIFT;
    unit->secondary_skillset = party->secondary_skillset;
    unit->reaction_ability = party->reaction_ability[0] + (party->reaction_ability[1] << 8);
    unit->support_ability = party->support_ability[0] + (party->support_ability[1] << 8);
    unit->movement_ability = party->movement_ability[0] + (party->movement_ability[1] << 8);
    main_util_copy_byte_data(party->equipment, unit->equipment, UNIT_EQUIPMENT_SLOT_COUNT);
    unit->experience = party->experience;
    unit->level = party->level;
    unit->original_brave = unit->brave = party->bravery;
    unit->original_faith = unit->faith = party->faith;
    main_util_copy_byte_data(party->raw_stats, unit->raw_stats, 0xf);
    for (i = 0; i < UNIT_UNLOCKED_JOB_BYTE_COUNT; i++) {
        unit->unlocked_jobs[i] = party->unlocked_jobs[i];
    }
    main_util_copy_byte_data(party->learned_abilities, unit->learned_abilities, UNIT_LEARNED_ABILITY_BYTE_COUNT);
    main_util_copy_byte_data(party->job_levels, unit->job_levels, UNIT_JOB_LEVEL_BYTE_COUNT);
    /* Copy the adjacent current/total JP tables as their 80 serialized bytes. */
    main_util_copy_byte_data(party->job_points, unit->job_points, UNIT_JOB_POINT_TABLE_PAIR_BYTE_COUNT);
    main_util_copy_byte_data(party->name, unit->name, 0x10);
    unit->quote_name_id = party->name_id[0] + (party->name_id[1] << 8);
    main_util_clear_byte_data(&unit->ai_target_x, 7);
}
