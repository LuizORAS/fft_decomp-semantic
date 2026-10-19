#include "fft/battle.h"
#include "fft/battle_map.h"

/* Write the map cursor's square into destination as x, layer, y.
 *
 * Type debt (QUIRKS.md): callers pass an s16[3]; only vx/vy/vz are written through the SVECTOR
 * view. */
void battle_map_store_selected_tile_coordinates(SVECTOR* destination) {
    main_util_set_svector(destination, g_battle_cursor_x, g_battle_cursor_z, g_battle_cursor_y);
}
