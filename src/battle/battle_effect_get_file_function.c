#include "fft/battle.h"

battle_effect_callback_t battle_effect_get_file_function(s16 function_id) {
    return g_battle_effect_file_functions[function_id];
}
