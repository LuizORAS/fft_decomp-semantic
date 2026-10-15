#include "fft/world.h"
#include "psx/types.h"

/* Queue the confirm sound unless a menu restore is pending. */
void world_sound_set_effect_to_confirm_checked(void) {
    if (g_world_menu_restore_pending == 0) {
        g_world_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
    }
}
