#include "fft/open.h"

/* Clear every pending memory card event, the BIOS ones and the hardware ones; twin of
 * card_io_consume_all_events. */
void open_card_consume_all_events(void) {
    open_card_consume_bios_events();
    open_card_consume_hardware_events();
}
