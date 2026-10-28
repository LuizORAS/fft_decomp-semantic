#include "fft/main.h"
#include "fft/wldcore.h"

/* Load WLDCORE.BIN whole to the low overlay address and, when load_world is
 * set, WORLD.BIN (its whole window) to the world overlay address, then run WLDCORE
 * (wldcore_entrypoint) and return its world result. */
s32 main_overlay_exec_wldcore_and_world_bin(s32 load_world) {
    main_file_load_data_from_disc(DISC_SECTOR_WORLD_WLDCORE_BIN, DISC_SECTOR_COUNT_WORLD_WLDCORE_BIN,
        g_main_heap_low_overlay_load_address, MAIN_FILE_LOADING_DISPLAY_INIT);
    if (load_world != 0) {
        main_file_load_data_from_disc(DISC_SECTOR_WORLD_WORLD_BIN, RAM_WINDOW_BYTES_WORLD / DISC_SECTOR_BYTES,
            g_main_heap_world_overlay_load_address, MAIN_FILE_LOADING_DISPLAY_PRESERVE);
    }
    return wldcore_entrypoint();
}
