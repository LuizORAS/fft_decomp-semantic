#include "fft/battle.h"
#include "psx/types.h"

/* Type debt (QUIRKS.md): callers pass an s16[3]; only vx/vy/vz are written
 * through the SVECTOR view. */
void battle_map_store_selected_tile_coordinates(void* destination) {
    main_util_set_svector((SVECTOR*)destination, g_battle_cursor_x, g_battle_cursor_z, g_battle_cursor_y);
}
