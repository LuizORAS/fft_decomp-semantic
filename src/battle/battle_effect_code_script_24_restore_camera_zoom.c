#include "fft/battle.h"

/* Opcode 24: restore the camera zoom opcode 0f saved. */
s32 battle_effect_code_script_24_restore_camera_zoom(effect_record_t* record) {
    battle_camera_restore_zoom();
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
