#include "fft/battle.h"

/* Runs a record's script until an instruction yields (returns 1) or ends the
 * script (returns 0). */
s32 battle_effect_run_block(s16 index) {
    effect_record_t* rec;
    s16 result;

    rec = &g_effect_state_records[index];
    for (;;) {
        result = g_battle_effect_instructions[((battle_effect_script_header_t*)(rec->script + rec->pc))->word
            & EFFECT_SCRIPT_OPCODE_MASK](rec);
        if (result == EFFECT_SCRIPT_RESULT_ABORT) {
            return 0;
        }
        if (result == EFFECT_SCRIPT_RESULT_YIELD) {
            return 1;
        }
    }
}
