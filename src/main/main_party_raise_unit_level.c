#include "fft/battle.h"
#include "fft/main.h"
#include "psx/types.h"

/* Raise a roster unit by level_delta levels (up to 99), growing each 24-bit raw stat by
 * raw / (level + growth) per level with its job's growths, a zero growth counting as 1, as
 * main_unit_apply_level_growth does in battle. Returns the new level (the capped target itself when
 * it is not above the current one). The breeding code raises a new egg to a roster member's level. */
u32 main_party_raise_unit_level(party_data_t* party_unit, s32 level_delta) {
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
