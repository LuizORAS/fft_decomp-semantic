#include "fft/battle_effect.h"

/*
 * Secondary effect function 0x05: an erroneous
 * secondary animation that only steps its state so that a null animation can
 * finish cleanly.  Returns 1 to keep animating, 0 to stop.
 */
s32 effect_null_secondary_handler(void) {
    s32 result;

    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (g_effect_current_secondary->phase) {
    case EFFECT_SECONDARY_PHASE_INITIALIZING:
        g_effect_current_secondary->phase = EFFECT_SECONDARY_PHASE_EXECUTING;
        result = 1;
        break;
    case EFFECT_SECONDARY_PHASE_EXECUTING:
        g_effect_current_secondary->phase = EFFECT_SECONDARY_PHASE_FINALIZING;
        result = 1;
        break;
    case EFFECT_SECONDARY_PHASE_FINALIZING:
        result = 0;
        break;
    }
    return result;
}
