#include "fft/battle.h"
#include "psx/types.h"

/* DismissUnit (EVENT_OPCODE_DISMISS_UNIT): find the party member whose sprite set is `sprite_set` (from
 * roster slot 1: Ramza's slot 0 is never considered), return its equipped items to the inventory and
 * remove it from the party. Nothing happens when no member matches.
 *
 * The `j = 0;` before `none = ITEM_ID_NONE;` is load-bearing: it puts the induction variable's
 * initialisation first in the if-body so the branch's delay slot takes `move a0,zero` rather than
 * `li a2,0xff`. */
void battle_script_dismiss_unit_event_instruction(s32 sprite_set) {
    party_data_t* party;
    s32 i;
    s32 j;
    s32 none;
    u8 item_id;

    for (i = 1; i < PARTY_ROSTER_SLOT_COUNT; i++) {
        party = main_party_get_data_pointer(i);
        if (party->party_id != PARTY_ID_NONE && party->sprite_set == sprite_set) {
            break;
        }
    }
    if (i != PARTY_ROSTER_SLOT_COUNT) {
        j = 0;
        none = ITEM_ID_NONE;
        for (; j < UNIT_EQUIPMENT_SLOT_COUNT; j++) {
            item_id = party->equipment[j];
            if (item_id == ITEM_ID_NOTHING || item_id == none) {
                continue;
            }
            g_main_item_quantities[item_id]++;
        }
        main_party_remove_unit(i);
    }
}
