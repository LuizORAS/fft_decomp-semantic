#include "fft/open.h"

s32 open_card_check_slots(void) {
    s32 card_found = 0;
    s32 result;

    open_card_consume_all_events();

    open_card_set_slot(1);
    result = open_card_poll_info_until_stable(3);
    if (result == CARD_IO_RESULT_COMPLETE || result == CARD_IO_RESULT_NEW_CARD) {
        card_found = 1;
    }

    open_card_set_slot(0);
    result = open_card_poll_info_until_stable(3);
    if (result == CARD_IO_RESULT_COMPLETE || result == CARD_IO_RESULT_NEW_CARD) {
        card_found = 1;
    }

    return card_found;
}
