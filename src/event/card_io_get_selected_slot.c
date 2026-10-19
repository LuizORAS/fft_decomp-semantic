#include "fft/event_card.h"
#include "psx/types.h"

/* Return the selected memory card slot (g_card_io_selected_slot). */
s32 card_io_get_selected_slot(void) {
    return g_card_io_selected_slot;
}
