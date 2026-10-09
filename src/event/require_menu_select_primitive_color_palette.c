#include "fft/event_require.h"
#include "psx/types.h"

void require_menu_select_primitive_color_palette(
    world_menu_palette_primitives_t* output, const battle_menu_status_panel_frame_config_t* context) {
    if ((context->style == 1) || (g_battle_menu_input_lock_mode == 1)) {
        battle_menu_init_primitive_colors_palette_bank_1(output);
    } else {
        battle_menu_init_primitive_colors_palette_bank_0(output);
    }
}
