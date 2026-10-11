#include "fft/battle.h"

/* Opcode 0b: set the camera rotation from the three halfwords that follow. */
s32 battle_effect_code_script_0b_set_camera_rotation(effect_record_t* record) {
    SVECTOR rotation;

    setVector(&rotation, (u16)battle_effect_get_current_script_instruction(record)->vector.x,
        (u16)battle_effect_get_current_script_instruction(record)->vector.y,
        (u16)battle_effect_get_current_script_instruction(record)->vector.z);
    battle_camera_set_rotation(&rotation);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
