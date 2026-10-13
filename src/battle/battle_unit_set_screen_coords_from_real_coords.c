#include "fft/battle_unit.h"

/* Sets the integer screen position from the 20.12 fixed-point position. */
void battle_unit_set_screen_coords_from_real_coords(battle_unit_t* unit) {
    unit->screen.vx = unit->real.vx / ONE;
    unit->screen.vy = unit->real.vy / ONE;
    unit->screen.vz = unit->real.vz / ONE;
}
