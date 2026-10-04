#include "fft/main.h"

/* Load WORLD.BIN (sector 0x14925) without running it, with loading display mode mode - 1
 * (mode 0 shows no Now Loading message). The title's new-game party setup calls it. */
void main_overlay_load_world_bin(s32 mode) {
    main_file_load_data_from_disc(0x14925, 0x1E0, g_main_heap_world_overlay_load_address, mode - 1);
}
