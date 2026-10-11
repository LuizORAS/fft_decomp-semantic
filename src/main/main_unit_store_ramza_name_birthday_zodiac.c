#include "fft/main.h"

/* For a unit with one of Ramza's three identities, copy the name, birthday and
 * zodiac sign from the first roster record that has one of them. */
void main_unit_store_ramza_name_birthday_zodiac(battle_stats_t* unit) {
    s32 slot;
    party_data_t* party;
    u32 birthday;

    if (unit->character_identity > CHARACTER_IDENTITY_RAMZA_CHAPTER_4) {
        return;
    }
    for (slot = 0; slot < PARTY_ROSTER_SLOT_COUNT; slot++) {
        party = main_party_get_data_pointer(slot);
        if (party->party_id == PARTY_ID_NONE) {
            continue;
        }
        if (party->sprite_set > CHARACTER_IDENTITY_RAMZA_CHAPTER_4) {
            continue;
        }
        birthday = (party->birthday_day + (party->zodiac << 8)) & PARTY_BIRTHDAY_DAY_MASK;
        unit->birthday.birthday = birthday;
        unit->birthday.zodiac = party->zodiac >> PARTY_ZODIAC_SHIFT;
        main_util_copy_byte_data(party->name, unit->name, 16);
        return;
    }
}
