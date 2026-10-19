#include "fft/main.h"

/* Clear the four BIOS card events (done, error, timeout, new card) by
 * testing each. */
void card_io_consume_bios_events(void) {
    TestEvent(g_main_card_bios_events[MAIN_CARD_EVENT_BIOS_DONE]);
    TestEvent(g_main_card_bios_events[MAIN_CARD_EVENT_BIOS_ERROR]);
    TestEvent(g_main_card_bios_events[MAIN_CARD_EVENT_BIOS_TIMEOUT]);
    TestEvent(g_main_card_bios_events[MAIN_CARD_EVENT_BIOS_NEW_CARD]);
}
