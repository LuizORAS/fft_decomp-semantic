#include "fft/battle.h"
#include "psx/types.h"

s32 battle_unit_build_deployed_units_data(battle_deployed_coords_t* formation, s32 unused_1) {
    return battle_unit_init_deployed_units_data(formation, 0);
}
