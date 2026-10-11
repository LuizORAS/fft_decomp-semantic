#include "fft/battle.h"
#include "psx/types.h"

/* Sets the 20.12 fixed-point position from the integer screen position. */
void battle_unit_set_real_coords_from_screen_coords(battle_unit_misc_data_t* unit) {
    unit->real.vx = unit->screen.vx << 12;
    unit->real.vy = unit->screen.vy << 12;
    unit->real.vz = unit->screen.vz << 12;
}
