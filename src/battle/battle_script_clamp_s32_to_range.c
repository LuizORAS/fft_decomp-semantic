#include "psx/types.h"

/* Clamp `*value` into minimum..maximum in place; ChangeStats keeps HP within the unit's maximum and
 * Brave, Faith and CT within 0-100 with it. */
void battle_script_clamp_s32_to_range(s32* value, s32 minimum, s32 maximum) {
    s32 current;

    current = *value;
    if (current < minimum) {
        *value = minimum;
        return;
    }
    if (maximum < current) {
        *value = maximum;
    }
}
