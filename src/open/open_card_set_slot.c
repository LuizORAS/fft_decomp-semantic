#include "fft/open.h"
#include "psx/types.h"

/* Select the memory card slot (port) the title's card check talks to (g_open_card_slot). */
void open_card_set_slot(u8 slot) {
    g_open_card_slot = slot;
}
