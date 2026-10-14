#include "fft/wldcore.h"
#include "psx/types.h"

/* Branch to section operand 1 when flag operand 0 of the reading's local flags
 * (g_wldcore_sound_novel_local_flags) equals `expected`; otherwise go on. Opcodes 19 and 20.
 *
 * The target keeps the instruction record address in $s1 across the wldcore_bits_test call instead of
 * rebuilding it at each byte load. GCC folds a symbol address straight into both `lbu`s, so the empty
 * tied `__asm__` (which emits no instruction) is what forces the base into its own pseudo; no plain C
 * spelling of the two loads reproduces it. */
void wldcore_sound_novel_branch_if_local_flag(s32 expected) {
    wldcore_sound_novel_instruction_bytes_t* instruction = &g_wldcore_sound_novel_state.instruction.bytes;
    u16 flags;

    __asm__("" : "=r"(instruction) : "0"(instruction));

    if (wldcore_bits_test(g_wldcore_sound_novel_local_flags, instruction->operand_0) == expected) {
        flags = g_wldcore_sound_novel_record.state_flags | WLDCORE_SOUND_NOVEL_STATE_BRANCH;
        g_wldcore_sound_novel_record.section = instruction->operand_1;
    } else {
        flags = g_wldcore_sound_novel_record.state_flags | WLDCORE_SOUND_NOVEL_STATE_FETCH;
    }
    g_wldcore_sound_novel_record.state_flags = flags;
}
