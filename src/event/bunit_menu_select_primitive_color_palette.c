#include "fft/event_bunit.h"
#include "psx/types.h"

/* Select the menu-primitive palette bank from the panel style
 * or the global event mode. The palette
 * initializer receives `primitives` unchanged in $a0. */
void bunit_menu_select_primitive_color_palette(
    world_menu_palette_primitives_t* primitives, const world_status_thread_t* thread) {
    if (thread->style == 1 || g_battle_menu_input_lock_mode == 1) {
        battle_menu_init_primitive_colors_palette_bank_1(primitives);
    } else {
        battle_menu_init_primitive_colors_palette_bank_0(primitives);
    }
}
