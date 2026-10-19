#include "fft/battle_effect.h"

/* Return the phase of secondary effect secondary_id (battle_effect_secondary_phase_e).
 * battle_effect_update_file_load waits in its last state for the ability's charge effect to reach
 * BATTLE_EFFECT_SECONDARY_PHASE_ENDED. */
battle_effect_secondary_phase_e battle_effect_get_secondary_phase(u8 secondary_id) {
    return g_battle_effect_secondary_data[secondary_id].phase;
}
