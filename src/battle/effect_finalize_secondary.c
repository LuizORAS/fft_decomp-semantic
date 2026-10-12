#include "fft/battle_effect.h"

void effect_finalize_secondary(u8 secondary_id) {
    g_effect_secondary_data[secondary_id].phase = EFFECT_SECONDARY_PHASE_FINALIZING;
}
