#include "fft/main.h"
#include "psx/types.h"

/* Raise a roster unit by level_delta levels (up to 99), growing each 24-bit raw stat by
 * raw / (level + growth) per level with its job's growths, a zero growth counting as 1, as
 * main_unit_apply_level_growth does in battle. Returns the new level (the capped target itself when
 * it is not above the current one). The breeding code raises a new egg to a roster member's level. */
u32 main_party_raise_unit_level(party_unit_t* party_unit, s32 level_delta) {
    u32 current_level = party_unit->level;
    u32 level = current_level + level_delta;
    job_stat_growth_t* growth_table;
    unit_raw_stat_bytes_t* raw_stats;
    s32 levels_to_gain;
    s32 level_index;
    s32 i;
    u32 raw;
    u32 divisor;

    raw_stats = party_unit->raw_stats;
    growth_table = g_main_job_data_pointer[party_unit->job_id].stat_growths;
    if (level > UNIT_LEVEL_MAX) {
        level = UNIT_LEVEL_MAX;
    }
    levels_to_gain = level - current_level;
    if (levels_to_gain <= 0) {
        return level;
    }
    for (level_index = 0; level_index < levels_to_gain; level_index++) {
        for (i = 0; i < UNIT_RAW_STAT_COUNT; i++) {
            level = party_unit->level;
            raw = raw_stats[i].low + (raw_stats[i].middle << 8) + (raw_stats[i].high << 16);
            if (growth_table[i].growth == 0) {
                divisor = level + 1;
            } else {
                divisor = level + growth_table[i].growth;
            }
            raw += raw / divisor;
            if (raw > 0xffffff) {
                raw = 0xffffff;
            }
            raw_stats[i].low = raw;
            raw_stats[i].middle = raw >> 8;
            raw_stats[i].high = raw >> 16;
        }
        party_unit->level++;
    }
    return party_unit->level;
}
