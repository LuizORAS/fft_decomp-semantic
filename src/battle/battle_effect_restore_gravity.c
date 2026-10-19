#include "fft/battle_effect.h"

/* Put back the gravity battle_effect_save_gravity saved. */
void battle_effect_restore_gravity(void) {
    g_battle_effect_gravity = g_battle_effect_saved_gravity;
}
