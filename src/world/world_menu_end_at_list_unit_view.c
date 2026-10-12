#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_end_at_list_unit_view, for WORLD's copy of the AT list; nothing in the
 * game calls it. */
void world_menu_end_at_list_unit_view(void) {
    g_world_menu_at_list_unit_view_ended = 1;
}
