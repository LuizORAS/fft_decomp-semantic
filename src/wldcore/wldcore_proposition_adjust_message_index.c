#include "fft/wldcore.h"

/* A report message in a unit's voice: `base` plus the proposition data's adjustment (record 2) for
 * the unit's Faith and Brave bands (below 36, 66 and 101 give 0, 1 and 2), plus 3 for a unit not
 * flagged male. A value of 101 or more leaves its band unset, as in the target. */
s32 wldcore_proposition_adjust_message_index(s32 base, s32 party_index) {
    s32 faith_band;
    s32 bravery_band;
    u32 is_not_male;
    party_data_t* party;

    party = wldcore_unit_get_party_data_pointer(party_index);
    if (party->bravery < 0x65) {
        bravery_band = 2;
    }
    if (party->bravery < 0x42) {
        bravery_band = 1;
    }
    if (party->bravery < 0x24) {
        bravery_band = 0;
    }
    if (party->faith < 0x65) {
        faith_band = 2;
    }
    if (party->faith < 0x42) {
        faith_band = 1;
    }
    if (party->faith < 0x24) {
        faith_band = 0;
    }
    is_not_male = party->gender_flags & UNIT_FLAG_MALE;
    is_not_male = is_not_male < 1;
    return base
        + wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_MESSAGE_ADJUSTMENTS)
              ->message_adjustments[faith_band * 3 + bravery_band]
        + is_not_male * 3;
}
