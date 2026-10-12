#include "fft/world.h"

void world_formation_restore_saved_system_flags(void) {
    g_main_system_flags = g_world_formation_saved_system_flags;
}
