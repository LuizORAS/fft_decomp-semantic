#include "fft/battle_camera.h"
#include "fft/battle_effect.h"

/* Opcode 0c: save the camera rotation (opcode 0d restores it). */
effect_script_result_e battle_effect_code_script_0c_save_camera_rotation(effect_record_t* record) {
    camera_copy_rotation();
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
