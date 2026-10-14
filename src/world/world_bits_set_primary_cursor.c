#include "fft/world.h"
#include "psx/types.h"

/* Selects the primary bitstream and resets its next operation to bit 7. */
void world_bits_set_primary_cursor(u8* ptr) {
    g_world_primary_bit_cursor_reset = 1;
    g_world_primary_bit_cursor = ptr;
}
