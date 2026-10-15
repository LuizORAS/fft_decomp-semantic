#include "fft/battle.h"
#include "psx/types.h"

/* Point g_battle_effect_frame_data back at g_battle_effect_frame_data_start. Only the RESTART_SCRIPT
 * phase of battle_effect_update_stage calls it, and nothing enters that phase or writes the start pointer,
 * which holds 0 (QUIRKS.md). */
void battle_effect_reset_frame_data_pointer(void) {
    g_battle_effect_frame_data = g_battle_effect_frame_data_start;
}
