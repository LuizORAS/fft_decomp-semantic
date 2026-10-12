#include "fft/wldcore.h"

/* Push the map path level (WLDCORE_MENU_LEVEL_MAP_PATH) for a location request that draws or erases
 * the route between two locations: the route is loaded as a path animation
 * (wldcore_map_prepare_path_animation), and the location label and the parent's window are hidden
 * until wldcore_menu_step_map_path_level ends it.
 *
 * Every menu-stack and window-record store must stay a bare-symbol reference so
 * it keeps the four-instruction lui/addiu/addu/sw $at expansion the target uses
 * at all six indexed-global sites. */
void wldcore_menu_push_map_path_level(s32 from_location, s32 to_location, s32 erase) {
    s32 depth;
    s32 window_index;

    wldcore_map_prepare_path_animation(from_location, to_location);
    depth = g_wldcore_menu_stack_depth;
    g_wldcore_menu_stack_records_next[depth].path.from_location = from_location;
    g_wldcore_menu_stack_records_next[depth].path.to_location = to_location;
    g_wldcore_menu_stack_records_next[depth].path.erase = erase;
    g_wldcore_menu_stack_records_next[depth].path.pending_sound = 1;
    g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
    window_index = g_wldcore_menu_stack_records_next[depth - 1].window_index;
    g_wldcore_menu_stack_types[depth + 1] = WLDCORE_MENU_LEVEL_MAP_PATH;
    g_wldcore_window_records[window_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_menu_stack_depth = depth + 1;
}
