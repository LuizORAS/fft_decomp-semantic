#include "fft/event_card.h"
#include "psx/types.h"

/* Point the primary bit reader (card_bits_read_primary) at `data`: the next read starts at the most
 * significant bit of its first byte. */
void card_bits_init_primary_reader(const u8* data) {
    g_card_bits_primary_reset = 1;
    g_card_bits_primary_cursor = data;
}
