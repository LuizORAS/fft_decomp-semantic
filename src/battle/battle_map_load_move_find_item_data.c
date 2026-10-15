#include "fft/battle.h"

/* Make a map's four Move-Find tiles (their items and traps) the current ones, in
 * g_battle_current_map_move_find_item_data. */
void battle_map_load_move_find_item_data(map_move_find_item_data_t* map_data) {
    g_battle_current_map_move_find_item_data = *map_data;
}
