#include "fft/battle.h"

/* Opcode 0b: set the camera rotation from the three halfwords that follow. */
s32 battle_effect_code_script_0b_set_camera_rotation(effect_record_t* record) {
    SVECTOR rotation;

    rotation.vx = *(u16*)(record->pc + (s32)record->script + 2);
    rotation.vy = *(u16*)(record->pc + (s32)record->script + 4);
    rotation.vz = *(u16*)(record->pc + (s32)record->script + 6);
    battle_camera_set_rotation(&rotation);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
