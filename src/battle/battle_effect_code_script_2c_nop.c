#include "fft/battle_effect.h"

/* Opcode 2c: nothing. */
effect_script_result_e battle_effect_code_script_2c_nop(effect_record_t* record) {
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
