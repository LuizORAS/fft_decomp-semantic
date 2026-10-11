#include "fft/battle.h"

/* Opcode 0e: set the camera zoom from the three halfwords that follow (ONE is
 * no zoom). */
s32 battle_effect_code_script_0e_set_camera_zoom(effect_record_t* record) {
    VECTOR zoom;

    zoom.vx = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc)->vector.x.signed_value;
    zoom.vy = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc)->vector.y.signed_value;
    zoom.vz = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc)->vector.z.signed_value;
    battle_camera_set_zoom(&zoom);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
