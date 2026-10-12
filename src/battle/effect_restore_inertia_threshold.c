#include "fft/battle_effect.h"

void effect_restore_inertia_threshold(void) {
    g_effect_inertia_threshold = g_effect_saved_inertia_threshold;
}
