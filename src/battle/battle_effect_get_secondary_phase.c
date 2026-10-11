#include "fft/battle.h"

s32 battle_effect_get_secondary_phase(u8 secondary_id) {
    return g_battle_effect_secondary_data[secondary_id].phase;
}
