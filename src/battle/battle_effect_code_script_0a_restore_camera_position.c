#include "fft/battle.h"

/* Opcode 0a: restore the camera position opcode 09 saved. */
s32 battle_effect_code_script_0a_restore_camera_position(effect_record_t* record) {
    battle_camera_restore_current_real_coords();
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
