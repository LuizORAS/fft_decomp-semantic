#include "fft/event_bunit.h"
#include "psx/types.h"

/* Point BUNIT's bit reader (bunit_bits_read) at `data`: the next read starts at the most significant
 * bit of its first byte. BUNIT reads glyph pixels and the unlocked-job bits with it. */
void bunit_bits_init_reader(const u8* data) {
    g_bunit_bits_reader_reset = 1;
    g_bunit_bits_cursor = data;
}
