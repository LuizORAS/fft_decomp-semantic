#include "fft/menu.h"

/* Reads a `count`-bit value, most significant bit first, from the secondary
 * bitstream. The first read after a reset starts at bit 7 of the current
 * byte. */
u32 menu_bits_read_secondary(s32 count) {
    s32 source_bit;
    s32 next_source_bit;
    const u8* source;
    s32 source_byte;
    u32 result;
    u32 return_value;

    return_value = 0;
    if (count != 0) {
        result = 0;
        if (g_menu_bits_secondary_reset != 0) {
            g_menu_bits_secondary_index = 7;
            g_menu_bits_secondary_reset = 0;
        }
        count--;
        return_value = -1;
        if (count != return_value) {
            do {
                source = g_menu_bits_secondary_source;
                source_bit = g_menu_bits_secondary_index;
                source_byte = *source;
                next_source_bit = source_bit - 1;
                g_menu_bits_secondary_index = next_source_bit;
                if ((source_byte >> source_bit) & 1) {
                    result |= 1 << count;
                }
                count--;
                if (next_source_bit < 0) {
                    g_menu_bits_secondary_index = 7;
                    g_menu_bits_secondary_source = source + 1;
                }
            } while (count != -1);
        }
        return_value = result;
    }
    return return_value;
}
