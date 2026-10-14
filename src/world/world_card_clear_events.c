#include "fft/world.h"

/* Clear every pending memory card event, the BIOS ones and the hardware ones; twin of
 * card_io_consume_all_events. */
void world_card_clear_events(void) {
    world_card_consume_bios_events();
    world_card_consume_hardware_events();
}
