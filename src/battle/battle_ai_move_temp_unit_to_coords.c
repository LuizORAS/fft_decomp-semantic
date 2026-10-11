#include "fft/battle.h"

/* Position the temporary simulation unit at the supplied AI coordinates. */
void battle_ai_move_temp_unit_to_coords(battle_ai_coords_t* coord) {
    g_battle_ai_data_base.acting_unit->x = coord->bytes.x;
    g_battle_ai_data_base.acting_unit->position.y = coord->bytes.y;
    g_battle_ai_data_base.acting_unit->position.higher_elevation = coord->bytes.elevation;
}
