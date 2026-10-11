#include "fft/battle.h"

/* Opcode 0b: set the camera rotation from the three halfwords that follow. */
s32 battle_effect_code_script_0b_set_camera_rotation(effect_record_t* record) {
    SVECTOR rotation;

    rotation.vx = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc)->vector.x.unsigned_value;
    rotation.vy = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc)->vector.y.unsigned_value;
    rotation.vz = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc)->vector.z.unsigned_value;
    battle_camera_set_rotation(&rotation);
    record->pc += 8;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
