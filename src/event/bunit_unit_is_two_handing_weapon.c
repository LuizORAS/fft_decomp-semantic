#include "fft/event_bunit.h"
#include "psx/types.h"

/* Returns 1 when the unit holds a single weapon in two hands: exactly one of
 * the two hand slots is occupied, its item is below the throwable range, and
 * the weapon either forces two hands or allows them while `allow_secondary`
 * (the Two Hands support ability) is set. */
s32 bunit_unit_is_two_handing_weapon(s16* slots, s32 allow_secondary) {
    s32 ret = 0;
    s32 id = 0;

    if (slots[0] != 0) {
        if (slots[1] == 0) {
            id = slots[0] & 0x3FF;
        }
    } else if (slots[1] != 0) {
        id = slots[1] & 0x3FF;
    }
    if (id != 0 && id < ITEM_ID_THROWABLE_FIRST) {
        u8 flags = g_main_item_weapon_data[id].flags;
        if (flags & WEAPON_FLAG_FORCED_TWO_HANDS) {
            ret = 1;
        } else if (allow_secondary != 0 && (flags & WEAPON_FLAG_TWO_HANDS_COMPATIBLE)) {
            ret = 1;
        }
    }
    return ret;
}
