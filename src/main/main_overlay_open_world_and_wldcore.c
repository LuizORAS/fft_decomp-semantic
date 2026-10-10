#include "fft/main.h"
#include "fft/wldcore.h"

s32 main_overlay_open_world_and_wldcore(s32 load_world) {
    main_file_load_data_from_disc(DISC_SECTOR_WORLD_WLDCORE_BIN, 0xDC, g_main_heap_low_overlay_load_address, 0);
    if (load_world != 0) {
        main_file_load_data_from_disc(DISC_SECTOR_WORLD_WORLD_BIN, 0x1E0, g_main_heap_world_overlay_load_address, 1);
    }
    return wldcore_entrypoint();
}
