#include "fft/battle.h"
#include "fft/main.h"
#include "psx/types.h"

/* Raises a roster unit by `level_delta` levels (capped at 99), growing each
 * 24-bit raw stat by raw / (level + growth) per level, and returns the new
 * level.  The growth table is the job's growths_multipliers pairs. */
u32 main_party_level_unit_to_target(party_data_t* party_unit, s32 level_delta) {
    u32 current_level = party_unit->level;
    u32 level = current_level + level_delta;
    u8* growth_table;
    u8* raw_stats;
    s32 levels_to_gain;
    s32 level_index;
    s32 i;
    u32 raw;
    u32 divisor;

    raw_stats = party_unit->raw_stats;
    growth_table = g_main_job_data_pointer[party_unit->job_id].growths_multipliers;
    if (level >= 100) {
        level = 99;
    }
    levels_to_gain = level - current_level;
    if (levels_to_gain <= 0) {
        return level;
    }
    for (level_index = 0; level_index < levels_to_gain; level_index++) {
        for (i = 0; i < UNIT_RAW_STAT_COUNT; i++) {
            level = party_unit->level;
            raw = raw_stats[i * 3] + (raw_stats[i * 3 + 1] << 8) + (raw_stats[i * 3 + 2] << 16);
            if (growth_table[i * 2] == 0) {
                divisor = level + 1;
            } else {
                divisor = level + growth_table[i * 2];
            }
            raw += raw / divisor;
            if (raw > 0xffffff) {
                raw = 0xffffff;
            }
            raw_stats[i * 3] = raw;
            raw_stats[i * 3 + 1] = raw >> 8;
            raw_stats[i * 3 + 2] = raw >> 16;
        }
        party_unit->level++;
    }
    return party_unit->level;
}
