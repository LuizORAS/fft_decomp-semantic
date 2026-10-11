#include "fft/main.h"
#include "fft/wldcore.h"

/* Load WLDCORE.BIN (0xdc sectors at 0x14849) to the low overlay address and, when load_world is
 * set, WORLD.BIN (0x1e0 sectors) to the world overlay address, then run WLDCORE
 * (wldcore_entrypoint) and return its world result. */
s32 main_overlay_exec_wldcore_and_world_bin(s32 load_world) {
    main_file_load_data_from_disc(DISC_SECTOR_WORLD_WLDCORE_BIN, 0xDC, g_main_heap_low_overlay_load_address, 0);
    if (load_world != 0) {
        main_file_load_data_from_disc(DISC_SECTOR_WORLD_WORLD_BIN, 0x1E0, g_main_heap_world_overlay_load_address, 1);
    }
    return wldcore_entrypoint();
}
