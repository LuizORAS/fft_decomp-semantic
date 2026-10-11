#include "fft/battle.h"

/* Opcode 0e: set the camera zoom from the three halfwords that follow (ONE is
 * no zoom). */
s32 battle_effect_code_script_0e_set_camera_zoom(effect_record_t* record) {
    VECTOR zoom;

    zoom.vx = battle_effect_get_current_script_instruction(record)->vector.x;
    zoom.vy = battle_effect_get_current_script_instruction(record)->vector.y;
    zoom.vz = battle_effect_get_current_script_instruction(record)->vector.z;
    battle_camera_set_zoom(&zoom);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
