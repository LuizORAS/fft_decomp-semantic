#include "fft/world.h"

/* Give a status panel's line box its dimmed colours when the panel style is 1 or menu input is
 * locked, its normal colours otherwise. */
void world_menu_select_primitive_color_palette(
    world_menu_palette_primitives_t* menu, const wldcore_menu_status_thread_t* input) {
    if (input->style == 1 || g_menu_input_lock_mode == 1) {
        world_menu_init_primitive_colors_dimmed(menu);
        return;
    }
    world_menu_init_primitive_colors_normal(menu);
}
