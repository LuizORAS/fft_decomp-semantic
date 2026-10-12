#include "fft/battle_camera.h"
#include "fft/battle_effect.h"
#include "psx/libgpu.h"

/* Opcode 0e: set the camera zoom from the three 16-bit values that follow (ONE is
 * no zoom). */
effect_script_result_e battle_effect_code_script_0e_set_camera_zoom(effect_record_t* record) {
    VECTOR zoom;

    setVector(&zoom, battle_effect_get_current_script_instruction(record)->vector.x,
        battle_effect_get_current_script_instruction(record)->vector.y,
        battle_effect_get_current_script_instruction(record)->vector.z);
    battle_camera_set_zoom(&zoom);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
