#include "fft/battle.h"

/* Opcode 08: move the camera to the position in the three halfwords that
 * follow (x, y, z in world units). */
s32 battle_effect_code_script_08_set_camera_position(effect_record_t* record) {
    VECTOR coords;

    coords.vx = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc)->vector.x.signed_value << 12;
    coords.vy = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc)->vector.y.signed_value << 12;
    coords.vz = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc)->vector.z.signed_value << 12;
    battle_camera_set_current_real_coords(&coords);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
