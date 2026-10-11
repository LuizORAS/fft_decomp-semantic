#include "fft/main.h"
#include "psx/types.h"

/* Give a roster unit the JP of a finished proposition, in its current job's slot (slot 0 for a
 * special job): current and total JP each stop at 9999, and the job level and unlocked jobs are
 * derived again. Returns -1 for a negative amount, a slot past the roster, an empty slot or a monster,
 * else 0. wldcore_proposition_step_result_level pays each participant this way. */
s32 main_job_add_proposition_jp(u32 party_index, s32 jp) {
    s32 slot;
    party_data_t* party;
    u8 job;
    s32 byte_index;
    s32 total;
    s32 level;
    s32 levels;
    u8 level_byte;

    if (jp < 0) {
        return -1;
    }
    if (party_index >= 20) {
        return -1;
    }
    party = main_party_get_data_pointer(party_index);
    if (party->party_id == PARTY_ID_NONE) {
        return -1;
    }
    if (party->gender_flags & UNIT_FLAG_MONSTER) {
        return -1;
    }
    job = party->job_id;
    if (job >= JOB_ID_GENERIC_FIRST && job <= JOB_ID_GENERIC_LAST) {
        slot = job - JOB_ID_GENERIC_FIRST;
    } else {
        slot = 0;
    }

    /* Each serialized JP entry is a little-endian byte pair. */
    byte_index = slot * 2;
    total = party->job_points[byte_index] + (party->job_points[byte_index + 1] << 8) + jp;
    if (total >= 10000) {
        total = 9999;
    }
    party->job_points[byte_index] = total;
    party->job_points[byte_index + 1] = total / 256;

    total = party->total_job_points[byte_index] + (party->total_job_points[byte_index + 1] << 8) + jp;
    if (total >= 10000) {
        total = 9999;
    }
    party->total_job_points[byte_index] = total;
    party->total_job_points[byte_index + 1] = total / 256;
    level = main_job_calculate_level(total & 0xFFFF);

    levels = party->job_levels[slot / 2];
    level_byte = level;
    if ((slot & 1) != 0) {
        levels = level + (levels & 0xF0);
    } else {
        levels = (levels & 0xF) | (level_byte << 4);
    }
    party->job_levels[slot / 2] = levels;

    main_job_store_unlock_bitset(
        party->unlocked_jobs, main_job_calculate_unlocked(party->job_levels, party->gender_flags));
    return 0;
}
