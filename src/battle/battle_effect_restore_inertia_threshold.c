#include "fft/battle_effect.h"

/* Put back the inertia threshold battle_effect_save_inertia_threshold saved. */
void battle_effect_restore_inertia_threshold(void) {
    g_battle_effect_inertia_threshold = g_battle_effect_saved_inertia_threshold;
}
