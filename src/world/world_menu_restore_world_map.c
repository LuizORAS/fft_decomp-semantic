#include "fft/world.h"

/* Put back the system flags world_menu_hide_world_map saved. */
void world_menu_restore_world_map(void) {
    g_main_system_flags = g_world_menu_saved_system_flags;
}
