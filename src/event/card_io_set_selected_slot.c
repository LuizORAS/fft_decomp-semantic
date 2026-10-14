#include "fft/event_card.h"
#include "psx/types.h"

/* Select the memory card slot (port) the card routines talk to (g_card_io_selected_slot). */
void card_io_set_selected_slot(s32 value) {
    g_card_io_selected_slot = value;
}
