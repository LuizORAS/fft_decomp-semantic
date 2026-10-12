#include "fft/world.h"

void world_menu_clear_selection_records(void) {
    memset(g_world_menu_selection_records, 0, sizeof(g_world_menu_selection_records));
}
