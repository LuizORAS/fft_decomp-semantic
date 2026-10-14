#include "fft/world.h"

/* a0 is forwarded unchanged to the selected palette helper. */
void world_menu_select_primitive_color_palette(menu_palette_primitives_t* menu, menu_status_thread_t* input) {
    if (input->style == 1 || g_menu_input_lock_mode == 1) {
        world_menu_init_primitive_colors_dimmed(menu);
        return;
    }
    world_menu_init_primitive_colors_normal(menu);
}
