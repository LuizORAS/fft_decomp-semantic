#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Point the primary bit reader (jobstts_bits_read_primary) at `data`: the next read starts at the most
 * significant bit of its first byte. */
void jobstts_bits_init_primary_reader(const u8* data) {
    g_jobstts_bits_primary_reset = 1;
    g_jobstts_bits_primary_source = data;
}
