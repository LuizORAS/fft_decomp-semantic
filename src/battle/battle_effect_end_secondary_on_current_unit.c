#include "fft/battle_effect.h"

/* Timer and animation use field-address offsets to retain their store order. */

/* Start BATTLE_EFFECT_SECONDARY_ANIMATION_ENDING in a fresh secondary effect
 * record and return its id. Its null handler only steps through the phases, so
 * the record lasts two more frames. The arrow handler starts it when the arrow
 * lands without a hit. */
s32 battle_effect_end_secondary_on_current_unit(void) {
    s32 secondary_id = battle_effect_allocate_secondary_slot() & 0xFF;
    s32 record_offset = secondary_id * sizeof(battle_effect_secondary_data_t);
    u8 function_id;

    *(s16*)((u8*)&g_battle_effect_secondary_data[0].timer + record_offset) = 0;
    *(u16*)((u8*)&g_battle_effect_secondary_data[0].animation_id + record_offset)
        = BATTLE_EFFECT_SECONDARY_ANIMATION_ENDING;
    function_id = g_battle_effect_ending_secondary_function_id;
    g_battle_effect_secondary_data[secondary_id].phase = BATTLE_EFFECT_SECONDARY_PHASE_INITIALIZING;
    g_battle_effect_secondary_data[secondary_id].function_id = function_id;
    return secondary_id;
}
