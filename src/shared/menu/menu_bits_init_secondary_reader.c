#include "fft/menu.h"

/* Point the secondary bit reader (menu_bits_read_secondary) at `data`: the next read starts at the
 * most significant bit of its first byte. */
void menu_bits_init_secondary_reader(const u8* data) {
    g_menu_bits_secondary_reset = 1;
    g_menu_bits_secondary_source = data;
}
