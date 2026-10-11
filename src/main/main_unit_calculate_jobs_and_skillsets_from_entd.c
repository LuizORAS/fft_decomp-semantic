#include "fft/battle.h"
#include "fft/main.h"

/* Set a unit's job levels, unlocked jobs, job and skillsets from its ENTD record and load the job's
 * data (main_unit_copy_job_data). A human whose record names a career job (job_unlock below 20) gets
 * that job's unlock requirements as job levels, job_level as the job's own level, and at least level 1
 * in every job but the one its sex cannot take (Bard, Dancer); any other unit unlocks no job. A
 * primary skillset in the record (not 0 or 0xff) replaces the job's and sets ability_outcome to 0xff,
 * which the turn clock clears before the first turn. The secondary skillset is the record's, or for
 * ENTD_VALUE_RANDOM the skillset of a random unlocked job (none for a monster, or when it is the
 * primary). With no ENTD record (the Morbol transformation) the unit becomes a monster of its current
 * job with no secondary skillset. */
void main_unit_calculate_jobs_and_skillsets_from_entd(battle_stats_t* unit, const entd_unit_t* entd) {
    s32 i;
    u32 unlocked;
    u32 flags;
    u8 selected;
    u16 skillset;
    u8 levels;

    main_util_clear_byte_data(unit->job_levels, UNIT_JOB_LEVEL_BYTE_COUNT);
    unlocked = 0;
    if (!entd) {
        main_unit_copy_job_data(unit);
        unit->secondary_skillset = 0;
        unit->unit_flags = UNIT_FLAG_MONSTER;
        main_job_store_unlock_bitset(unit->unlocked_jobs, 0);
    } else {
        flags = entd->unit_flags;
        if (!(flags & UNIT_FLAG_MONSTER)) {
            selected = entd->job_unlock;
            if (selected < UNIT_CAREER_JOB_COUNT) {
                levels = entd->job_level & UNIT_JOB_LEVEL_LOW_NIBBLE_MASK;
                if (!(selected & 1))
                    levels <<= 4;
                unit->job_levels[selected / UNIT_JOBS_PER_LEVEL_BYTE] = levels;
                for (i = 0; i < UNIT_JOB_LEVEL_BYTE_COUNT; i++) {
                    if (selected)
                        levels = g_main_job_unlock_requirements[selected - 1][i];
                    else {
                        levels = UNIT_JOB_LEVEL_ONE_LOW_NIBBLE;
                        if (i != 0)
                            levels = UNIT_JOB_LEVEL_ONE_BOTH_NIBBLES;
                    }
                    levels |= unit->job_levels[i];
                    if (!(levels & UNIT_JOB_LEVEL_LOW_NIBBLE_MASK))
                        levels |= UNIT_JOB_LEVEL_ONE_LOW_NIBBLE;
                    if (!(levels & UNIT_JOB_LEVEL_HIGH_NIBBLE_MASK))
                        levels |= UNIT_JOB_LEVEL_ONE_HIGH_NIBBLE;
                    unit->job_levels[i] = levels;
                }
                if (flags & UNIT_FLAG_FEMALE)
                    unit->job_levels[UNIT_JOB_LEVEL_BYTE_INDEX_CALCULATOR_BARD] &= UNIT_JOB_LEVEL_HIGH_NIBBLE_MASK;
                if (flags & UNIT_FLAG_MALE)
                    unit->job_levels[UNIT_JOB_LEVEL_BYTE_INDEX_DANCER_MIME] &= UNIT_JOB_LEVEL_LOW_NIBBLE_MASK;
                unlocked = main_job_calculate_unlocked(unit->job_levels, flags);
            }
        }
        main_job_store_unlock_bitset(unit->unlocked_jobs, unlocked);
        unit->job_id = entd->job_id;
        main_unit_copy_job_data(unit);
        selected = entd->primary_skillset;
        if (selected != SKILLSET_ID_NONE && selected != 0) {
            unit->primary_skillset = selected;
            unit->ability_outcome = 0xff;
        }
        selected = entd->secondary_skillset;
        if (selected == ENTD_VALUE_RANDOM) {
            if (unit->unit_flags & UNIT_FLAG_MONSTER)
                unit->secondary_skillset = 0;
            else {
                /* The job-level temporary holds the random job id here. */
                levels = main_job_get_random_unlocked(unit);
                if (levels)
                    skillset = g_main_job_data_pointer[levels].skillset;
                else
                    skillset = levels;
                if (skillset == unit->primary_skillset)
                    skillset = 0;
                unit->secondary_skillset = skillset;
            }
        } else
            unit->secondary_skillset = selected;
    }
}
