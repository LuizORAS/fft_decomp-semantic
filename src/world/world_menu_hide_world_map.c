#include "fft/world.h"

/* Save g_main_system_flags (world_menu_restore_world_map puts them back) and hide the world map
 * for a full-screen menu: no dots, routes, background image, display objects, context value, date or
 * location marker, with the display off. The formation and card requests use it. */
void world_menu_hide_world_map(void) {
    g_world_menu_saved_system_flags = g_main_system_flags;
    g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_MAP_DOTS | MAIN_SYSTEM_FLAG_HIDE_MAP_ROUTES
        | MAIN_SYSTEM_FLAG_DISPLAY_OFF | MAIN_SYSTEM_FLAG_NO_BACKGROUND_IMAGE | MAIN_SYSTEM_FLAG_HIDE_DISPLAY_OBJECTS
        | MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE | MAIN_SYSTEM_FLAG_HIDE_CALENDAR_DATE
        | MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
}
