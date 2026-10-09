#include "fft/world.h"

/* Takes world_gfx_init_image_loading's arguments: WORLD stubs the BATTLE mode-1
 * loader (battle_menu_display_projected_action_effect) with this no-op. */
void world_noop_800e90ec(POLY_FT4* unused_primitive, const world_image_location_t* unused_base_load,
    const world_image_location_t* unused_base_screen, const world_gfx_image_load_parameters_t* unused_parameters) {
}
