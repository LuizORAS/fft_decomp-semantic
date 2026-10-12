#include "fft/wldcore.h"

/* Load picture set `set` of WLDPIC.BIN into the picture buffer, skipping the transfer when it is
 * already resident: the locations', the job reports', the treasures' and lands' and the sound novels'
 * pictures. */
void wldcore_gfx_load_picture_set(s32 set) {
    if (set != g_wldcore_loaded_picture_set) {
        wldcore_wait_for_file_load();
        g_wldcore_loaded_picture_set = set;
        wldcore_file_request_read(&g_main_file_cd_state,
            g_wldcore_picture_sector_offsets[set] + DISC_SECTOR_WORLD_WLDPIC_BIN,
            g_wldcore_picture_sector_offsets[set + 1] - g_wldcore_picture_sector_offsets[set],
            g_wldcore_picture_buffer);
    }
}
