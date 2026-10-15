#include "fft/battle.h"

/* Return native callback callback_id of g_battle_effect_native_callbacks, for script opcodes 06 and
 * 07. Its 198 entries are fixed addresses in the effect overlay area (0x801c2500 to 0x801c69c4), so a script
 * can only use the ids whose address holds a function of its own file. */
void* battle_effect_get_native_callback(s16 callback_id) {
    return g_battle_effect_native_callbacks[callback_id];
}
