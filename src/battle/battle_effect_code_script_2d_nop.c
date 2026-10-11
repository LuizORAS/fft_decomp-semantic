#include "fft/battle.h"

/* Opcode 2d: nothing. */
effect_script_result_e battle_effect_code_script_2d_nop(effect_record_t* record) {
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
