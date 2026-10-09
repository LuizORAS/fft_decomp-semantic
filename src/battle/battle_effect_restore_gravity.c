#include "fft/battle.h"

void battle_effect_restore_gravity(void) {
    g_battle_effect_gravity = g_battle_effect_saved_gravity;
}
