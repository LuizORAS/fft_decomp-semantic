#include "fft/wldcore.h"

/* Stream the world map's texture (WLDTEX.TM2, 0x86 sectors) into VRAM and wait for it:
 * wldcore_file_poll_vram_image_stream loads each sector's rectangles as it arrives. A restart skips
 * it, since the texture is still in VRAM.
 *
 * The target frame is 0x20 rather than the 0x18 this body needs: 16 bytes of
 * outgoing arguments plus the ra/s0 save pair leave 8 bytes of unreferenced
 * locals, so the original declared an 8-byte stack object it never used. Its
 * type is unknown; the array below reproduces the slot exactly. */
void wldcore_map_load_texture(void) {
    s32 unused_stack_slot[2];

    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_RESTART) == 0) {
        wldcore_file_request_read(&g_main_file_cd_state, DISC_SECTOR_WORLD_WLDTEX_TM2, 0x86, g_wldcore_scratch_buffer);
        while (g_main_file_cd_state.state != MAIN_FILE_LOAD_STATE_IDLE) {
            wldcore_file_poll_vram_image_stream(&g_main_file_cd_state);
        }
    }
}
