#include "fft/battle.h"

/* Set secondary effect secondary_id to BATTLE_SECONDARY_EFFECT_FINALIZING so that its handler winds it
 * down: battle_effect_update_file_load ends the ability's charge effect this way once its countdown has run
 * out. */
void battle_effect_finalize_secondary(u8 secondary_id) {
    g_battle_effect_secondary_data[secondary_id].phase = BATTLE_SECONDARY_EFFECT_FINALIZING;
}
