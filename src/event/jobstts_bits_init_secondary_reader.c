#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Point the secondary bit reader (jobstts_bits_read_secondary) at `data`: the next read starts at the
 * most significant bit of its first byte. */
void jobstts_bits_init_secondary_reader(const u8* data) {
    g_jobstts_bits_secondary_reset = 1;
    g_jobstts_bits_secondary_source = data;
}
