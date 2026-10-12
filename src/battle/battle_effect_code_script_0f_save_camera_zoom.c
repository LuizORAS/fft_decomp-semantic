#include "fft/battle_camera.h"
#include "fft/battle_effect.h"

/* Opcode 0f: save the camera zoom (opcode 24 restores it). */
effect_script_result_e battle_effect_code_script_0f_save_camera_zoom(effect_record_t* record) {
    battle_camera_copy_zoom();
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
