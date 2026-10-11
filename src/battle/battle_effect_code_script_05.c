#include "fft/battle.h"

/* Opcode 05: store the high nibble of the instruction's second byte in
 * D_801BF000, which nothing reads. Every retail script begins with it. */
effect_script_result_e battle_effect_code_script_05(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction = battle_effect_get_current_script_instruction(record);

    D_801BF000 = instruction->header.packed_parameters >> 4;
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
