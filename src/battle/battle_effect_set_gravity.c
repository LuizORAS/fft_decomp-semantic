#include "fft/battle_effect.h"

void battle_effect_set_gravity(vec3s32_t* src) {
    g_battle_effect_gravity = *src;
}
