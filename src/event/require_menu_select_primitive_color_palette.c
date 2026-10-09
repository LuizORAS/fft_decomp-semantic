#include "fft/event_require.h"
#include "psx/types.h"

void require_menu_select_primitive_color_palette(
    world_menu_palette_primitives_t* output, const world_status_thread_t* context) {
    if ((context->style == 1) || (g_battle_menu_input_lock_mode == 1)) {
        battle_menu_init_primitive_colors_dimmed(output);
    } else {
        battle_menu_init_primitive_colors_normal(output);
    }
}
