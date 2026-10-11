#include "fft/main.h"

/* Generate a new unit of unit_type (MAIN_UNIT_TYPE_*) in the first empty ordinary
 * roster slot (0..15), with palette 0. Returns the slot, or -1 when all sixteen
 * are taken. */
s32 main_party_generate_unit(s32 unit_type) {
    party_data_t* party_data[PARTY_ROSTER_SLOT_COUNT];
    s32 party_index;
    s32 palette = 0;

    for (party_index = 0; party_index < PARTY_ROSTER_SLOT_COUNT; party_index++) {
        party_data[party_index] = main_party_get_data_pointer(party_index);
    }

    for (party_index = 0; party_index < PARTY_MEMBER_SLOT_COUNT; party_index++) {
        if (party_data[party_index]->party_id == PARTY_ID_NONE) {
            party_data[party_index]->party_id = party_index;
            party_data[party_index]->palette = palette;
            main_unit_generate_out_of_battle(party_data[party_index], unit_type);
            return party_index;
        }
    }

    return -1;
}
