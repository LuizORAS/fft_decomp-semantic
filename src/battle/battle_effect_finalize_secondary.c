#include "fft/battle_effect.h"

void battle_effect_finalize_secondary(u8 secondary_id) {
    g_battle_effect_secondary_data[secondary_id].phase = BATTLE_SECONDARY_EFFECT_FINALIZING;
}
