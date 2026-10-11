#include "fft/event_bunit.h"
#include "psx/types.h"

/* Derive a party record's unlocked-job bitset from its job levels and sex
 * (main_job_calculate_unlocked with only the male and female flags, so a monster record counts as
 * sexless) and store it as three bytes, most significant first. Nothing references it. */
void bunit_job_calculate_and_store_unlocks(party_data_t* unit, u8* job_unlocks) {
    s32 unlocks
        = main_job_calculate_unlocked(unit->job_levels, unit->gender_flags & (UNIT_FLAG_FEMALE | UNIT_FLAG_MALE));

    job_unlocks[0] = unlocks >> 16;
    job_unlocks[1] = unlocks >> 8;
    job_unlocks[2] = unlocks;
}
