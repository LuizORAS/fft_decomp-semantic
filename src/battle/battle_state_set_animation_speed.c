#include "fft/battle.h"
#include "psx/types.h"

s32 battle_state_set_animation_speed(s32 speed) {
    if (speed == 1 || speed == 2) {
        g_main_system_animation_speed = speed;
        return 0;
    }
    return -1;
}
