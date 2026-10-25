#include "fft/battle_event.h"
#include "fft/battle_menu.h"
#include "fft/menu.h"

/* Give the menu primitives their dimmed colors when the panel style (the calling thread's mode field)
 * is 1 or the menu input lock mode is 1, their normal colors otherwise. The palette initializer
 * receives `primitives` unchanged in $a0. Several overlays compile this text. */
void menu_select_status_primitive_color_palette(
    world_menu_palette_primitives_t* primitives, const wldcore_menu_status_thread_t* thread) {
    if (thread->style == 1 || g_menu_input_lock_mode == 1) {
        battle_menu_init_primitive_colors_dimmed(primitives);
    } else {
        battle_menu_init_primitive_colors_normal(primitives);
    }
}
