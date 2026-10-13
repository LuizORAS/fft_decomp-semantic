#include "fft/main.h"
#include "fft/open.h"
#include "psx/types.h"

/* Poll the four card hardware events and report which one fired, or pending when
 * none fired. A fired event is consumed along with the rest of the set. */
s32 open_card_poll_hardware_event(void) {
    s32 result = CARD_IO_RESULT_PENDING;

    if (TestEvent(g_main_card_bios_events[MAIN_CARD_EVENT_HARDWARE_DONE]) == 1)
        result = CARD_IO_RESULT_COMPLETE;
    else if (TestEvent(g_main_card_bios_events[MAIN_CARD_EVENT_HARDWARE_ERROR]) == 1)
        result = CARD_IO_RESULT_ERROR;
    else if (TestEvent(g_main_card_bios_events[MAIN_CARD_EVENT_HARDWARE_TIMEOUT]) == 1)
        result = CARD_IO_RESULT_TIMEOUT;
    else if (TestEvent(g_main_card_bios_events[MAIN_CARD_EVENT_HARDWARE_NEW_CARD]) == 1)
        result = CARD_IO_RESULT_NEW_CARD;
    if (result != CARD_IO_RESULT_PENDING)
        card_io_consume_hardware_events();
    return result;
}
