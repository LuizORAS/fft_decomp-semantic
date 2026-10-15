#include "fft/wldcore.h"

/* Set up one world map visit, in this order: the state (wldcore_system_init_world_map_state), the input
 * repeat, the map texture, the GPU and ordering tables, the window pools, the tile grid, WORLD's
 * menus and any pending date advance, the location dots, the player's marker and the camera, the
 * root menu level, the first screen (wldcore_system_init_screen) and the level the visit opens with
 * (wldcore_menu_restore_level_on_world_entry).
 * The matched range includes the jr delay slot at 0x80067cb0. */
void wldcore_system_init_subsystems(void) {
    SetDispMask(0);
    wldcore_system_init_world_map_state();
    wldcore_input_init_repeat_state();
    wldcore_map_load_texture();
    wldcore_gfx_init_subsystem();
    wldcore_window_init_record_pools();
    wldcore_map_init_tile_atlas();
    wldcore_system_init_world_menus_and_date();
    wldcore_map_color_and_draw_dots();
    wldcore_system_init_stub();
    wldcore_map_init_location_marker_and_camera();
    wldcore_menu_reset_to_root_level();
    wldcore_system_init_screen();
    wldcore_menu_restore_level_on_world_entry();
}
