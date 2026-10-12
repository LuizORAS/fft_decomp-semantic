#include "fft/battle_ai.h"

/* Position the temporary simulation unit at the supplied AI coordinates. */
void ai_move_temp_unit_to_coords(ai_coords_t* coord) {
    g_ai_data_base.acting_unit->x = coord->bytes.x;
    g_ai_data_base.acting_unit->position.y = coord->bytes.y;
    g_ai_data_base.acting_unit->position.higher_elevation = coord->bytes.elevation;
}
