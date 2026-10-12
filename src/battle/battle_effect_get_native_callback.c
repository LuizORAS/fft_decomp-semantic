#include "fft/battle_effect.h"

/* Look up a native overlay callback by its resident catalog index. */
battle_effect_callback_t battle_effect_get_native_callback(s16 callback_id) {
    return g_battle_effect_native_callbacks[callback_id];
}
