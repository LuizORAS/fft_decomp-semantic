#include "fft/main.h"

/* Load WORLD.BIN without running it, with loading display mode mode - 1 (mode 0
 * shows no Now Loading message, mode 1 shows it). The title's new-game party setup calls it. */
void main_overlay_load_world_bin(s32 mode) {
    main_file_load_data_from_disc(DISC_SECTOR_WORLD_WORLD_BIN, RAM_WINDOW_BYTES_WORLD / DISC_SECTOR_BYTES,
        g_main_heap_world_overlay_load_address, mode - 1);
}
