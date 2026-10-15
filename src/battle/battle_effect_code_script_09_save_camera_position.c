#include "fft/battle.h"

/* Opcode 09: save the camera position (opcode 0a restores it). */
s32 battle_effect_code_script_09_save_camera_position(effect_record_t* record) {
    battle_camera_save_current_real_coords();
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
