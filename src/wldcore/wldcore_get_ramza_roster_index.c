#include "fft/wldcore.h"

/* Roster index (0..15) of the first occupied slot holding one of Ramza's
 * identities; 16 when there is none. */
s32 wldcore_get_ramza_roster_index(void) {
    s32 i;
    for (i = 0; i < PARTY_MEMBER_SLOT_COUNT; i++) {
        party_unit_t* party = wldcore_get_party_data_pointer(i);
        if (party->party_id != PARTY_ID_NONE && party->sprite_set != CHARACTER_IDENTITY_ENTD_NONE
            && party->sprite_set < CHARACTER_IDENTITY_RAMZA_END) {
            return i;
        }
    }
    return i;
}
