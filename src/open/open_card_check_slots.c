#include "fft/main.h"
#include "fft/open.h"

/* Return 1 when a memory card answers in either slot, 0 when neither does. Slot 1 is probed first,
 * then slot 0, which stays selected; complete and new card both count as a card. The title's New Game asks it
 * (open_title_start_new_game). */
s32 open_card_check_slots(void) {
    s32 card_present = 0;
    s32 result;

    card_io_consume_all_events();

    open_card_set_slot(1);
    result = open_card_poll_info_until_stable(3);
    if (result == CARD_IO_RESULT_COMPLETE || result == CARD_IO_RESULT_NEW_CARD) {
        card_present = 1;
    }

    open_card_set_slot(0);
    result = open_card_poll_info_until_stable(3);
    if (result == CARD_IO_RESULT_COMPLETE || result == CARD_IO_RESULT_NEW_CARD) {
        card_present = 1;
    }

    return card_present;
}
