#include "fft/main.h"
#include "psx/types.h"

/* Return a random unlocked job id, or 0 when none is unlocked. The draw covers the career jobs
 * before Mime (the last, never drawn); the base job slot gives a special character's own job (its
 * identity) and Squire for a generic unit. An ENTD unit's random secondary skillset comes from it
 * (main_unit_calculate_jobs_and_skillsets_from_entd).
 *
 * The ENTD caller explicitly normalizes the zero-extended job byte. */
s32 main_job_get_random_unlocked(const battle_stats_t* unit) {
    u8 candidates[UNIT_CAREER_JOB_INDEX_MIME];
    s32 job_mask = UNIT_UNLOCKED_JOB_SQUIRE;
    s32 candidate_count = 0;
    s32 job_index;
    s32 unlocked_jobs
        = (unit->career.unlocked_jobs[0] << 16) + (unit->career.unlocked_jobs[1] << 8) + unit->career.unlocked_jobs[2];

    for (job_index = 0; job_index < UNIT_CAREER_JOB_INDEX_MIME; job_index++, job_mask /= 2) {
        if (unlocked_jobs & job_mask) {
            u8 job_id = job_index + JOB_ID_SQUIRE;

            if (job_index == 0) {
                job_id = unit->character_identity;
                if (job_id >= CHARACTER_IDENTITY_SELECTOR_FIRST) {
                    job_id = JOB_ID_SQUIRE;
                }
            }
            candidates[candidate_count] = job_id;
            candidate_count++;
        }
    }

    if (candidate_count != 0) {
        s32 scaled_random = rand() * candidate_count;
        u8 candidate_index = (u32)scaled_random >> 15;

        if (scaled_random < 0) {
            candidate_index = (u32)(scaled_random + 0x7fff) >> 15;
        }
        return candidates[candidate_index];
    }
    return 0;
}
