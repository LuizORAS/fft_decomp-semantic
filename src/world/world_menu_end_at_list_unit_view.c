#include "fft/world.h"
#include "psx/types.h"

/* Raise g_world_menu_at_list_unit_view_ended. */
void world_menu_end_at_list_unit_view(void) {
    g_world_menu_at_list_unit_view_ended = 1;
}
