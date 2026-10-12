#include "fft/battle_effect.h"

void effect_reset_frame_data_pointer(void) {
    g_effect_frame_data = g_effect_frame_data_start;
}
