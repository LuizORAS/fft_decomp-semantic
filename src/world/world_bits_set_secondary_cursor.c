#include "fft/world.h"
#include "psx/types.h"

/* Selects the secondary read-only bitstream and resets it to bit 7. */
void world_bits_set_secondary_cursor(const u8* ptr) {
    g_world_secondary_bit_cursor_reset = 1;
    g_world_secondary_bit_cursor = ptr;
}
