#include "fft/battle.h"

/* Opcode 08: move the camera to the position in the three halfwords that
 * follow (x, y, z in world units). */
s32 battle_effect_code_script_08_set_camera_position(effect_record_t* record) {
    VECTOR coords;

    coords.vx = ((s16*)(record->pc + (s32)record->script))[1] << 12;
    coords.vy = ((s16*)(record->pc + (s32)record->script))[2] << 12;
    coords.vz = ((s16*)(record->pc + (s32)record->script))[3] << 12;
    battle_camera_set_current_real_coords(&coords);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
