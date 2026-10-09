#include "fft/battle.h"

s32 battle_effect_code_script_0b_set_camera_rotation(effect_record_t* effect) {
    SVECTOR rotation;

    rotation.vx = *(u16*)(effect->pc + (s32)effect->script + 2);
    rotation.vy = *(u16*)(effect->pc + (s32)effect->script + 4);
    rotation.vz = *(u16*)(effect->pc + (s32)effect->script + 6);
    battle_camera_set_rotation(&rotation);
    effect->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
