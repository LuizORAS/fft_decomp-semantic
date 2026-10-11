#include "fft/battle.h"
#include "psx/types.h"

/* Opcode 04: end the script; battle_effect_run_block's caller then frees the
 * record. */
s32 battle_effect_code_script_04_end(effect_record_t* unused_record) {
    return EFFECT_SCRIPT_RESULT_ABORT;
}
