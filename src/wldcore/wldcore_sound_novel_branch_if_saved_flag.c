#include "fft/wldcore.h"
#include "psx/types.h"

/* Branch to section operand 1 when flag operand 0 of g_main_sound_novel_saved_flags, the novels'
 * flags kept with the save, equals `expected`; otherwise go on. Opcodes 17 and 18. */
void wldcore_sound_novel_branch_if_saved_flag(s32 expected) {
    wldcore_sound_novel_instruction_bytes_t* instruction = &g_wldcore_sound_novel_state.instruction.bytes;
    u16 flags;

    /* Keep the instruction address live across the call for both operand loads. */
    __asm__("" : "=r"(instruction) : "0"(instruction));

    if (wldcore_test_bit(g_main_sound_novel_saved_flags, instruction->operand_0) == expected) {
        flags = g_wldcore_sound_novel_record.state_flags | WLDCORE_SOUND_NOVEL_STATE_BRANCH;
        g_wldcore_sound_novel_record.section = instruction->operand_1;
    } else {
        flags = g_wldcore_sound_novel_record.state_flags | WLDCORE_SOUND_NOVEL_STATE_FETCH;
    }
    g_wldcore_sound_novel_record.state_flags = flags;
}
