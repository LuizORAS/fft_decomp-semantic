#include "fft/main.h"

/* Clear every pending memory card event, the BIOS ones and the hardware ones
 * (card_io_consume_bios_events, card_io_consume_hardware_events). */
void card_io_consume_all_events(void) {
    card_io_consume_bios_events();
    card_io_consume_hardware_events();
}
