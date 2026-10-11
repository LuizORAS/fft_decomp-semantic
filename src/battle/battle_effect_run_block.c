#include "fft/battle.h"

/* Run until yield or abort; returns 1 on yield and 0 on abort. */
s32 battle_effect_run_block(s16 index) {
    effect_record_t* rec;
    /* A narrow result preserves the retail sign extension before the tests. */
    s16 result;

    rec = &g_effect_state_records[index];
    for (;;) {
        result = g_battle_effect_instructions[battle_effect_get_current_script_header(rec) & EFFECT_SCRIPT_OPCODE_MASK](
            rec);
        if (result == EFFECT_SCRIPT_RESULT_ABORT) {
            return 0;
        }
        if (result == EFFECT_SCRIPT_RESULT_YIELD) {
            return 1;
        }
    }
}
