#include "fft/battle_effect.h"

battle_effect_secondary_phase_e battle_effect_get_secondary_phase(u8 secondary_id) {
    return g_battle_effect_secondary_data[secondary_id].phase;
}
