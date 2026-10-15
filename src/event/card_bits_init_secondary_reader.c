#include "fft/event_card.h"
#include "psx/types.h"

/* Point the secondary bit reader (card_bits_read_secondary) at `data`: the next read starts at the
 * most significant bit of its first byte. */
void card_bits_init_secondary_reader(const u8* data) {
    g_card_bits_reader_2_reset = 1;
    g_card_bits_reader_2_stream = data;
}
