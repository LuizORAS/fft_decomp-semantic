#include "fft/battle_effect.h"

/* Add independent random offsets to three packed rotation components. */
void battle_effect_add_random_rotation_offsets(const SVECTOR* source, const SVECTOR* ranges, SVECTOR* destination) {
    u16 range;

    range = ranges->vx;
    destination->vx = source->vx + battle_effect_get_random_between_s16(-range, range);

    range = ranges->vy;
    destination->vy = source->vy + battle_effect_get_random_between_s16(-range, range);

    range = ranges->vz;
    destination->vz = source->vz + battle_effect_get_random_between_s16(-range, range);
}
