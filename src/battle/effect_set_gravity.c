#include "fft/battle_effect.h"

void effect_set_gravity(const vec3s32_t* src) {
    g_effect_gravity = *src;
}
