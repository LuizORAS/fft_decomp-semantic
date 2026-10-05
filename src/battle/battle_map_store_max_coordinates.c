#include "fft/battle.h"
#include "psx/types.h"

void battle_map_store_max_coordinates(SVECTOR* destination) {
    main_util_set_svector(destination, g_battle_map_max_x, 0, g_battle_map_max_y);
}
