#include "fft/battle.h"
#include "psx/types.h"

/* Write the cursor tile to `destination` as an SVECTOR: x, the layer in vy, and
 * the map y in vz. */
void battle_map_store_selected_tile_coordinates(void* destination) {
    main_util_set_svector(destination, g_battle_cursor_x, g_battle_cursor_z, g_battle_cursor_y);
}
