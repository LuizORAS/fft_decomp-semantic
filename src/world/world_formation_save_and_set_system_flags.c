#include "fft/world.h"

/* Save the system flags and hide the world map's layers (dots, routes,
 * background, display objects, context value, date and location marker)
 * with the display off, for a full-screen menu. */
void world_formation_save_and_set_system_flags(void) {
    g_world_formation_saved_system_flags = g_main_system_flags;
    g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_MAP_DOTS | MAIN_SYSTEM_FLAG_HIDE_MAP_ROUTES
        | MAIN_SYSTEM_FLAG_DISPLAY_OFF | MAIN_SYSTEM_FLAG_NO_BACKGROUND_IMAGE | MAIN_SYSTEM_FLAG_HIDE_DISPLAY_OBJECTS
        | MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE | MAIN_SYSTEM_FLAG_HIDE_CALENDAR_DATE
        | MAIN_SYSTEM_FLAG_HIDE_LOCATION_MARKER;
}
