#include "psx/suzuki.h"
#include "psx/types.h"

/* Modulator step: square wave between +amplitude and -amplitude, the sign
 * tracked in SUZUKI_MODULATOR_NEGATED. Returns the current value. */
s32 main_smd_modulator_step_alternating(suzuki_modulator_t* modulator) {
    s32 next_value;

    if (--modulator->timer == 0) {
        next_value = modulator->amplitude;
        modulator->timer = modulator->timer_reset;
        if ((modulator->flags & SUZUKI_MODULATOR_NEGATED) != 0)
            next_value = -next_value;
        modulator->value = next_value;
        modulator->flags ^= SUZUKI_MODULATOR_NEGATED;
    }
    return modulator->value;
}
