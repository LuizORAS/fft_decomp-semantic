#include "fft/battle.h"

/* Opcode 0b: set the camera rotation from the three halfwords that follow. */
s32 battle_effect_code_script_0b_set_camera_rotation(effect_record_t* record) {
    SVECTOR rotation;

    rotation.vx = ((battle_effect_script_vector_instruction_t*)(record->pc + (s32)record->script))->x.unsigned_value;
    rotation.vy = ((battle_effect_script_vector_instruction_t*)(record->pc + (s32)record->script))->y.unsigned_value;
    rotation.vz = ((battle_effect_script_vector_instruction_t*)(record->pc + (s32)record->script))->z.unsigned_value;
    battle_camera_set_rotation(&rotation);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
