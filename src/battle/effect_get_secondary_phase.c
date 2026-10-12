#include "fft/battle_effect.h"

effect_secondary_phase_e effect_get_secondary_phase(u8 secondary_id) {
    return g_effect_secondary_data[secondary_id].phase;
}
