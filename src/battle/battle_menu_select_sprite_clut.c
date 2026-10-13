#include "fft/battle_event.h"
#include "fft/battle_menu.h"
#include "fft/thread.h"

/* Select the CLUT of a menu sprite: the dimmed one while the
 * previous event thread is running or event mode is active, otherwise the
 * normal one.
 *
 * Target 0x801383c8. Structurally the BATTLE twin of
 * world_menu_select_icon_cluts. */
void battle_menu_select_sprite_clut(SPRT* sprite) {
    if (battle_thread_is_previous_running() != 0 || g_battle_menu_input_lock_mode == 1) {
        sprite->clut = 0x7D3C;
    } else {
        sprite->clut = 0x7C3C;
    }
}
