#include "psx/suzuki.h"
#include "psx/types.h"

/* Deactivates a modulator. */
void suzuki_smd_modulator_deactivate(suzuki_modulator_t* modulator) {
    modulator->flags &= ~SUZUKI_MODULATOR_ACTIVE;
}
