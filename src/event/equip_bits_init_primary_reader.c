#include "fft/event_equip.h"

/* Point the primary bit reader (equip_bits_read_primary) at `data`: the next read starts at the most
 * significant bit of its first byte. */
void equip_bits_init_primary_reader(const u8* data) {
    g_equip_bits_reader_1_reset = 1;
    g_equip_bits_reader_1_stream = data;
}
