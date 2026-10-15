#include "fft/world.h"
#include "psx/libc.h"
#include "psx/types.h"

/* Advance egg hatch counters, then make the day's monster-breeding roll (wldcore_calendar_advance_day).
 *
 * An egg's counter is the low nibble of its birthday; at 0 it hatches as a monster born on the
 * current date. The roll, reseeded from the play time, needs a monster in the roster (eggs do not
 * count): a random byte below 6 breeds a rare offspring, below 0x10 an uncommon one and below 0x20 a
 * common one, so 32 in 256 days breed. The offspring job comes from a random roster monster's entry
 * in the breeding table (indexed from the Chocobo); the egg hatches in 2 to hatch_range days, takes a
 * random egg color and a monster name, and is raised to the level of a random non-egg roster
 * member. */
void world_unit_update_monster_breeding(s32 unused) {
    u8 monsters[0x18];
    u8 levels[0x18];
    s32 monster_count = 0;
    s32 unit_count = 0;
    s32 i = 0;
    /* The original reused two scalar temporaries throughout; splitting them
     * into typed locals changes the callee-saved allocation (n holds the
     * egg counter, month index, rarity class, offspring job and new slot;
     * value holds the birthday, the random draw and the saved name-buffer
     * pointer). */
    s32 value;
    s32 n;
    s32 day;
    s32 month;
    u8** name_buffer;
    party_data_t* unit;

    srand(g_main_system_play_time_frames * 100 + g_main_system_play_time_seconds * 10 + g_main_system_play_time_minutes
        + g_main_system_play_time_hours + g_main_system_play_time_frames);
    do {
        unit = main_party_get_data_pointer(i);
        if (unit->party_id != PARTY_ID_NONE) {
            if (unit->gender_flags & UNIT_FLAG_EGG) {
                n = unit->birthday_day & 0xF;
                n -= 1;
                unit->birthday_day = (unit->birthday_day & 0xF0) | n;
                if (n == 0) {
                    unit->gender_flags = UNIT_FLAG_MONSTER;
                    day = world_script_get_variable(EVENT_SCRIPT_VAR_DAY);
                    month = world_script_get_variable(EVENT_SCRIPT_VAR_MONTH);
                    value = 0;
                    for (n = 1; n < month; n++) {
                        value += g_world_month_lengths[n];
                    }
                    value += day;
                    value |= world_unit_get_zodiac_for_date(month, day) << 12;
                    unit->birthday_day = value;
                    unit->zodiac = value >> 8;
                }
            } else {
                levels[unit_count++] = unit->level;
                value = world_job_get_skillset(unit->job_id);
                if (value >= SKILLSET_ID_MONSTER_FIRST && value < SKILLSET_ID_END) {
                    monsters[monster_count++] = i;
                }
            }
        }
        i++;
    } while (i < PARTY_ROSTER_SLOT_COUNT);

    if (monster_count == 0) {
        return;
    }
    value = rand() & 0xFF;
    if (value < 6) {
        n = WORLD_MONSTER_BREED_RARITY_RARE;
    } else if (value < 0x10) {
        n = WORLD_MONSTER_BREED_RARITY_UNCOMMON;
    } else {
        n = value < 0x20;
    }
    if (n == WORLD_MONSTER_BREED_RARITY_NONE) {
        return;
    }
    monster_count = rand() % monster_count;
    n = g_world_unit_monster_breed_table[main_party_get_data_pointer(monsters[monster_count])->job_id - JOB_ID_CHOCOBO]
            .offspring_job_by_rarity[n - WORLD_MONSTER_BREED_RARITY_COMMON];
    i = rand() % g_world_unit_monster_breed_table[n - JOB_ID_CHOCOBO].hatch_range + 1;
    if (i < 2) {
        i = 2;
    }
    name_buffer = &g_world_text_section_pointers[TEXT_SECTION_ROSTER_UNIT_NAMES];
    value = (s32)*name_buffer;
    *name_buffer = g_world_text_section_pointers[TEXT_SECTION_UNIT_NAMES];
    n = main_party_create_monster_egg(n, i + i * 0x10, rand() % 4);
    *name_buffer = (u8*)value;
    if (n != -1) {
        unit = main_party_get_data_pointer(n);
        main_party_raise_unit_level(unit, levels[rand() % unit_count] - 1);
    }
}
