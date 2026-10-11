#include "fft/battle.h"

/* Opcode 0e: set the camera zoom from the three halfwords that follow (ONE is
 * no zoom). */
s32 battle_effect_code_script_0e_set_camera_zoom(effect_record_t* record) {
    VECTOR zoom;

    zoom.vx = *(s16*)(record->pc + (s32)record->script + 2);
    zoom.vy = *(s16*)(record->pc + (s32)record->script + 4);
    zoom.vz = *(s16*)(record->pc + (s32)record->script + 6);
    battle_camera_set_zoom(&zoom);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
