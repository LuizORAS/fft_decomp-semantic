#include "fft/battle_effect.h"

void effect_restore_gravity(void) {
    g_effect_gravity = g_effect_saved_gravity;
}
