#include "fft/main.h"

/* Add an egg of monster_job to the first empty ordinary roster slot. Returns the slot, or -1 when
 * there is none.
 *
 * egg_modifier is stored as the unit's birthday (values from 0x16e up become 1) and the zodiac sign
 * is derived from it; the breeding code passes the egg's days to hatch in both nibbles, and the low
 * one counts down (world_unit_update_monster_breeding). */
s32 main_party_create_monster_egg(s32 monster_job, s32 egg_modifier, s32 egg_color) {
    s32 party_index;
    party_data_t* party_data;
    u16 birthday;
    s32 zodiac;

    party_index = main_party_generate_unit(MAIN_UNIT_TYPE_MONSTER);
    if (party_index == -1) {
        return party_index;
    }
    party_data = main_party_get_data_pointer(party_index);
    party_data->job_id = monster_job;
    party_data->egg_color = egg_color;
    party_data->gender_flags |= UNIT_FLAG_EGG;
    birthday = egg_modifier;
    if (birthday >= 0x16e) {
        birthday = 1;
    }
    zodiac = main_unit_calculate_zodiac_symbol(birthday);
    party_data->birthday_day = birthday;
    party_data->zodiac = ((birthday & (PARTY_BIRTHDAY_DAY_HIGH_BIT_MASK << 8)) >> 8) + (zodiac << PARTY_ZODIAC_SHIFT);
    return party_index;
}
