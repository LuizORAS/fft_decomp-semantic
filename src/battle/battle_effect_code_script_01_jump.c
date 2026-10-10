#include "fft/battle.h"

/* Opcode 01: jump to the script offset in the second halfword. */
s32 battle_effect_code_script_01_jump(effect_record_t* record) {
    s32 pc = record->pc;
    s32 base = (s32)record->script;
    record->pc = *(u16*)(pc + base + 2);
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
