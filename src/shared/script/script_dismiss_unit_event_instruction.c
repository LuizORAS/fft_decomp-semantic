#include "fft/main.h"
#include "fft/script.h"
#include "psx/types.h"

/* DismissUnit (EVENT_OPCODE_DISMISS_UNIT): find the party member whose sprite set is `sprite_set` (from
 * roster slot 1: Ramza's slot 0 is never considered), return its equipped items to the inventory and
 * remove it from the party. Nothing happens when no member matches.
 *
 * The `slot = 0;` before `empty_item = ITEM_ID_NONE;` is load-bearing: it puts the induction variable's
 * initialization first in the if-body so the branch's delay slot takes `move a0,zero` rather than
 * `li a2,0xff`. */
void script_dismiss_unit_event_instruction(s32 sprite_set) {
    party_unit_t* party;
    s32 party_index = 1;
    s32 empty_party_id = PARTY_ID_NONE;
    s32 empty_item;
    s32 slot;
    u8 item;

    for (; party_index < PARTY_ROSTER_SLOT_COUNT; party_index++) {
        party = main_party_get_unit(party_index);
        if (party->party_id != empty_party_id && party->sprite_set == sprite_set) {
            break;
        }
    }
    if (party_index != PARTY_ROSTER_SLOT_COUNT) {
        slot = 0;
        empty_item = ITEM_ID_NONE;
        for (; slot < UNIT_EQUIPMENT_SLOT_COUNT; slot++) {
            item = party->equipment[slot];
            if (item != ITEM_ID_NOTHING && item != empty_item) {
                g_main_item_quantities[item]++;
            }
        }
        main_party_remove_unit(party_index);
    }
}
