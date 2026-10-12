#include "fft/battle_unit.h"

/* Sets the integer screen position from the 20.12 fixed-point position. */
void battle_unit_set_screen_coords_from_real_coords(battle_unit_misc_data_t* misc) {
    misc->screen.vx = misc->real.vx / ONE;
    misc->screen.vy = misc->real.vy / ONE;
    misc->screen.vz = misc->real.vz / ONE;
}
