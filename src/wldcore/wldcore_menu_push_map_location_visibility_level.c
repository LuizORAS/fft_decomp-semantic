#include "fft/wldcore.h"

/* Push the location visibility level (WLDCORE_MENU_LEVEL_MAP_LOCATION_VISIBILITY) for a location
 * request that shows a location's dot or, with `erase`, hides it; the location label and the
 * parent's window are hidden until the step handler ends the level.
 *
 * Needs aspsx 2.21: under 2.34 maspsx folds %lo into the store displacement
 * and the function is eight instructions short. Every
 * menu-stack store below must stay a bare-symbol reference so it keeps the
 * 2.21 four-instruction lui/addiu/addu/sw expansion. */
void wldcore_menu_push_map_location_visibility_level(s32 dot_index, s32 erase) {
    s32 depth;
    s32 window_index;

    depth = g_wldcore_menu_stack_depth;
    g_wldcore_menu_stack_records_next[depth].visibility.dot_index = dot_index;
    g_wldcore_menu_stack_records_next[depth].visibility.erase = erase;
    g_wldcore_menu_stack_records_next[depth].visibility.delay = 0x10;
    g_wldcore_menu_stack_records_next[depth].visibility.pending_sound = 1;
    g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
    window_index = g_wldcore_menu_stack_records_next[depth - 1].window_index;
    g_wldcore_menu_stack_types[depth + 1] = WLDCORE_MENU_LEVEL_MAP_LOCATION_VISIBILITY;
    g_wldcore_window_records[window_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_menu_stack_depth = depth + 1;
}
