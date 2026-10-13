#include "fft/battle_ai.h"

/* Position the temporary simulation unit at the supplied AI coordinates. */
void ai_move_temp_unit_to_coords(const ai_coords_t* coord) {
    g_ai_data_base.acting_unit->x = coord->fields.x;
    g_ai_data_base.acting_unit->position.y = coord->fields.y;
    g_ai_data_base.acting_unit->position.higher_elevation = coord->fields.elevation;
}
