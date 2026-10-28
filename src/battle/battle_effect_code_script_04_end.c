#include "fft/battle_effect.h"

/* Opcode 04: end the script; battle_effect_run_block's caller then frees the
 * record. */
effect_script_result_e battle_effect_code_script_04_end(battle_effect_record_t* unused_record) {
    return EFFECT_SCRIPT_RESULT_ABORT;
}
