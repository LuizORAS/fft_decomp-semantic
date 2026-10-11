#include "fft/battle.h"

/* Opcode 0e: set the camera zoom from the three halfwords that follow (ONE is
 * no zoom). */
s32 battle_effect_code_script_0e_set_camera_zoom(effect_record_t* record) {
    VECTOR zoom;

    zoom.vx = ((battle_effect_script_vector_instruction_t*)(record->pc + (s32)record->script))->x.signed_value;
    zoom.vy = ((battle_effect_script_vector_instruction_t*)(record->pc + (s32)record->script))->y.signed_value;
    zoom.vz = ((battle_effect_script_vector_instruction_t*)(record->pc + (s32)record->script))->z.signed_value;
    battle_camera_set_zoom(&zoom);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
