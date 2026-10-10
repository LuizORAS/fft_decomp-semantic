#include "fft/battle.h"
#include "fft/main.h"

/* One job's row of battle_stats_t.learned_abilities. Indexing the rows
 * through this view keeps the row address (unit + job * 3 + 0x99) one
 * loop-invariant value, as in the target; a flat learned_abilities[job * 3 +
 * i / 8] adds the two indexes first. */
typedef struct unit_learned_ability_row {
    u8 bits[UNIT_LEARNED_ABILITY_BYTES_PER_JOB];
} unit_learned_ability_row_t;

/* Learns abilities for one job of a unit.
 * job_id 0x4a (Squire) stands for the unit's base job.  With an ENTD primary
 * skillset of 0 every ability of the unit's own job is learned; otherwise
 * each ability is rolled against its learn chance and bought with job JP.
 * The original reuses two scratch variables: `i` doubles as the unlocked-job
 * mask and `scratch` holds both the unlocked-job bits and the learn chance;
 * separate locals change the register allocation. */
void main_unit_learn_job_abilities(battle_stats_t* unit, s32 job_id, const entd_unit_t* entd) {
    s32 job_index;
    s32 skillset;
    s32 auto_learn;
    s32 learned;
    s32 i;
    u16 ability_id;
    s32 scratch;
    ability_data_t* ability;
    u8 character;
    s32 unit_flags;

    auto_learn = 0;
    job_index = job_id - JOB_ID_SQUIRE;
    if (job_id == JOB_ID_SQUIRE) {
        character = unit->character_identity;
        if (character < CHARACTER_IDENTITY_SELECTOR_FIRST
            || character >= CHARACTER_IDENTITY_SELECTOR_FIRST + CHARACTER_IDENTITY_GENERIC_HUMAN_COUNT) {
            if (character == CHARACTER_IDENTITY_MONSTER) {
                job_id = unit->job_id;
            } else {
                job_id = unit->character_identity;
            }
        }
    } else {
        i = 0x800000 >> job_index;
        scratch = (unit->unlocked_jobs[0] << 16) + (unit->unlocked_jobs[1] << 8) + unit->unlocked_jobs[2];
        if ((scratch & i) == 0) {
            return;
        }
    }

    unit_flags = unit->unit_flags;
    if (job_id == JOB_ID_BARD && (unit_flags & UNIT_FLAG_FEMALE)) {
        return;
    }
    if (job_id == JOB_ID_DANCER && (unit_flags & UNIT_FLAG_MALE)) {
        return;
    }

    if (job_id == unit->job_id) {
        skillset = unit->primary_skillset;
        if (entd->primary_skillset == 0) {
            auto_learn = 1;
        }
    } else {
        skillset = g_main_job_data_pointer[job_id].skillset;
    }
    for (i = 0; i < SKILLSET_ABILITY_LIST_COUNT; i++) {
        learned = 0;
        ability_id = main_ability_get_id_from_skillset(skillset, i);
        if (ability_id != 0) {
            ability = &g_main_ability_data[ability_id];
            scratch = ability->learn_rate;
            if (auto_learn != 0) {
                learned = 1;
            } else if (main_util_roll_pass_fail(100, scratch) == 0) {
                u16 cost = ability->jp_cost;
                if (unit->job_points[job_index] >= cost) {
                    unit->job_points[job_index] -= cost;
                    ((unit_learned_ability_row_t*)unit->learned_abilities)[job_index].bits[i / 8] |= 0x80 >> (i % 8);
                    learned = 1;
                }
            }
            if (learned != 0) {
                ((unit_learned_ability_row_t*)unit->learned_abilities)[job_index].bits[i / 8] |= 0x80 >> (i % 8);
            }
        }
    }
}
