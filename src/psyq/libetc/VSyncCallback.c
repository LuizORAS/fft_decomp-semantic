#include "psx/libetc.h"

/* Set the VBlank callback in slot 0 (setIntrVSync, through the dispatch table). main_boot_run_startup
 * installs main_system_handle_vsync_callback, which advances rand and counts the play time. */
void VSyncCallback(void* callback) {
    g_psyq_etc_dispatch->vblank_callback(0, callback);
}
