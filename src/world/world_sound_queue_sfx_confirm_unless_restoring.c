#include "fft/world.h"

/* Queue the confirm sound unless a menu restore is pending. */
void world_sound_queue_sfx_confirm_unless_restoring(void) {
    if (g_world_menu_restore_pending == 0) {
        g_world_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
    }
}
