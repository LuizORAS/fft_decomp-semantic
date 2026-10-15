#include "fft/battle.h"

/* Same as battle_effect_save_gravity for g_battle_effect_inertia_threshold. */
void battle_effect_save_inertia_threshold(void) {
    g_battle_effect_saved_inertia_threshold = g_battle_effect_inertia_threshold;
}
