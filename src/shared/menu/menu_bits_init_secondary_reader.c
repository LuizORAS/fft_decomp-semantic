#include "fft/menu.h"

void menu_bits_init_secondary_reader(const u8* data) {
    g_menu_bits_secondary_reset = 1;
    g_menu_bits_secondary_source = data;
}
