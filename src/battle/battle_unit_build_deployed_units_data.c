#include "fft/battle_unit.h"
#include "psx/types.h"

/* Same as battle_unit_init_deployed_units_data for a normal deployment (mode 0); the second argument is
 * ignored. Step 8 of battle_map_step_init_sequence places the units this way. */
s32 battle_unit_build_deployed_units_data(unit_deployment_coordinates_t* formation, s32 unused_1) {
    return battle_unit_init_deployed_units_data(formation, 0);
}
