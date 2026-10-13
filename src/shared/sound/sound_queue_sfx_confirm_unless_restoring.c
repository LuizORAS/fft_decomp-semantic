#include "fft/main.h"
#include "fft/menu.h"
#include "fft/sound.h"

/* Queue the confirm sound unless a menu restore is pending
 * (g_menu_restore_pending). */
void sound_queue_sfx_confirm_unless_restoring(void) {
    if (g_menu_restore_pending == 0) {
        g_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
    }
}
