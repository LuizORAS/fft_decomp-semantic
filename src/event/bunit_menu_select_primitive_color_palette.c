#include "fft/event_bunit.h"
#include "psx/types.h"

/* Select the menu primitives' normal or dimmed colours from the panel style
 * or the global event mode. The palette
 * initializer receives `primitives` unchanged in $a0. */
void bunit_menu_select_primitive_color_palette(
    menu_palette_primitives_t* primitives, const menu_status_thread_t* thread) {
    if (thread->style == 1 || g_battle_menu_input_lock_mode == 1) {
        battle_menu_init_primitive_colors_dimmed(primitives);
    } else {
        battle_menu_init_primitive_colors_normal(primitives);
    }
}
