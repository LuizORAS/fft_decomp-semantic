#include "fft/battle.h"

/* Opcode 08: move the camera to the position in the three halfwords that
 * follow (x, y, z in world units). */
s32 battle_effect_code_script_08_set_camera_position(effect_record_t* record) {
    VECTOR coords;

    setVector(&coords, battle_effect_get_current_script_instruction(record)->vector.x << 12,
        battle_effect_get_current_script_instruction(record)->vector.y << 12,
        battle_effect_get_current_script_instruction(record)->vector.z << 12);
    battle_camera_set_current_real_coords(&coords);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
