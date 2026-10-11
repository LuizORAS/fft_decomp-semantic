#include "fft/battle.h"

/* Opcode 0b: set the camera rotation from the three halfwords that follow. */
s32 battle_effect_code_script_0b_set_camera_rotation(effect_record_t* record) {
    SVECTOR rotation;

    rotation.vx = (u16)battle_effect_get_current_script_instruction(record)->vector.x;
    rotation.vy = (u16)battle_effect_get_current_script_instruction(record)->vector.y;
    rotation.vz = (u16)battle_effect_get_current_script_instruction(record)->vector.z;
    battle_camera_set_rotation(&rotation);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
