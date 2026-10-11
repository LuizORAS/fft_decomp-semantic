#include "fft/main.h"
#include "psx/types.h"

/* Build a unit from party slot `slot`: the record's job levels and unlocked jobs are first
 * recomputed from its total JP, then the unit's per-battle state is cleared and it takes the record
 * (main_unit_copy_party_record), its job data, R/S/M flags, stats, equippable types, equipment
 * bonuses, Move and Jump with full HP and MP, and names in BATTLE's or WORLD's text
 * (use_world_text). Returns -1 for an empty or invalid slot, else 0. */
s32 main_unit_init_from_party_slot(battle_stats_t* unit, s32 slot, s32 use_world_text) {
    party_data_t* party_unit;
    u8* job_levels;

    g_main_unit_name_uses_world_text = use_world_text;
    party_unit = main_party_get_data_pointer(slot);
    if (party_unit == 0) {
        return -1;
    }
    if (party_unit->party_id == PARTY_ID_NONE) {
        return -1;
    }
    job_levels = party_unit->job_levels;
    main_unit_init_job_levels(party_unit->total_job_points, job_levels);
    main_job_store_unlock_bitset(
        party_unit->unlocked_jobs, main_job_calculate_unlocked(job_levels, party_unit->gender_flags));
    main_unit_reset_battle_state(unit);
    unit->formation_index = slot;
    main_unit_copy_party_record(unit, party_unit);
    main_unit_copy_job_data(unit);
    main_unit_enable_rsm_flags(unit);
    main_unit_calculate_actual_stats(unit, 0);
    main_unit_set_equippable_items(unit);
    main_unit_apply_equipment_move_jump_and_name(unit);
    return 0;
}
