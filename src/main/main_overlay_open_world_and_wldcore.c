#include "fft/main.h"
#include "fft/wldcore.h"

/* Load WLDCORE.BIN (0xdc sectors) to the low overlay address and, when load_world
 * is set, WORLD.BIN (0x1e0 sectors) to the world overlay address, then run
 * WLDCORE and return its result (see wldcore_entrypoint). */
s32 main_overlay_open_world_and_wldcore(s32 load_world) {
    main_file_load_data_from_disc(
        DISC_SECTOR_WORLD_WLDCORE_BIN, 0xDC, g_main_heap_low_overlay_load_address, MAIN_FILE_LOADING_DISPLAY_INIT);
    if (load_world != 0) {
        main_file_load_data_from_disc(DISC_SECTOR_WORLD_WORLD_BIN, 0x1E0, g_main_heap_world_overlay_load_address,
            MAIN_FILE_LOADING_DISPLAY_PRESERVE);
    }
    return wldcore_entrypoint();
}
