#include "fft/main.h"

/* Load WORLD.BIN alone. mode - 1 is the loading display mode of
 * main_file_build_header, so mode 1 shows the "Now Loading" screen. */
void main_overlay_open_world_bin(s32 mode) {
    main_file_load_data_from_disc(DISC_SECTOR_WORLD_WORLD_BIN, 0x1E0, g_main_heap_world_overlay_load_address, mode - 1);
}
