#include "fft/world.h"
#include "psx/types.h"

/* Derive a roster unit's unlocked-job bitset from its job levels and sex (main_job_calculate_unlocked
 * with only the male and female flags, so a monster record counts as sexless) and store it as three
 * bytes, most significant first, as party and formation records keep it. Nothing references it. */
void world_job_calculate_and_store_unlocks(party_data_t* party, u8 unlocked_jobs[3]) {
    s32 jobs
        = main_job_calculate_unlocked(party->job_levels, party->gender_flags & (UNIT_FLAG_FEMALE | UNIT_FLAG_MALE));
    unlocked_jobs[0] = jobs >> 16;
    unlocked_jobs[1] = jobs >> 8;
    unlocked_jobs[2] = jobs;
}
