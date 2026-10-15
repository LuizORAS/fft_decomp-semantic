#include "fft/battle.h"
#include "psx/types.h"

/* Point g_battle_effect_script at the script section of effect file effect_id, where
 * battle_effect_update_stage starts the root record. */
void battle_effect_set_script_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_battle_effect_file_headers[effect_id];
    g_battle_effect_script = (u8*)header + header->script;
}
