#include "fft/battle.h"

/* Opcode 0c: save the camera rotation (opcode 0d restores it). */
s32 battle_effect_code_script_0c_save_camera_rotation(effect_record_t* record) {
    battle_camera_save_rotation();
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
