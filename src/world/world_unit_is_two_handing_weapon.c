#include "fft/world.h"
#include "psx/types.h"

/* Returns 1 when the unit holds a single weapon in two hands: exactly one of
 * the two hand slots is occupied, its item is below the throwable range, and
 * the weapon either forces two hands or allows them while `two_hands_support`
 * (the Two Hands support ability) is set. */
s32 world_unit_is_two_handing_weapon(s16* equipment, s32 two_hands_support) {
    s32 result;
    s32 item_id;
    s16 right_hand;
    s16 left_hand;
    u8 flags;

    result = 0;
    right_hand = equipment[WORLD_EQUIPMENT_SLOT_RIGHT_HAND];
    item_id = ITEM_ID_NOTHING;
    if (right_hand != ITEM_ID_NOTHING) {
        if (equipment[WORLD_EQUIPMENT_SLOT_LEFT_HAND] == ITEM_ID_NOTHING) {
            item_id = right_hand & WORLD_ITEM_ID_MASK;
        }
    } else {
        left_hand = equipment[WORLD_EQUIPMENT_SLOT_LEFT_HAND];
        if (left_hand != ITEM_ID_NOTHING) {
            item_id = left_hand & WORLD_ITEM_ID_MASK;
        }
    }
    if (item_id != ITEM_ID_NOTHING && item_id < ITEM_ID_THROWABLE_FIRST) {
        flags = g_main_item_weapon_data[item_id].flags;
        if (flags & WEAPON_FLAG_FORCED_TWO_HANDS) {
            result = 1;
        } else if (two_hands_support != 0) {
            if (flags & WEAPON_FLAG_TWO_HANDS_COMPATIBLE) {
                result = 1;
            }
        }
    }
    return result;
}
