#include "fft/event_equip.h"
#include "psx/types.h"

/* Point the secondary bit reader (equip_bits_read_secondary) at `data`: the next read starts at the
 * most significant bit of its first byte. */
void equip_bits_init_secondary_reader(const u8* data) {
    g_equip_bits_reader_2_reset = 1;
    g_equip_bits_reader_2_stream = data;
}
