#include "fft/wldcore.h"
#include "psx/types.h"

/* Branch to section operand 2 when the read flag of section operand 1 of novel operand 0
 * (g_main_sound_novel_section_bits, set as a section is entered) equals `expected`; otherwise go on.
 * Opcodes 15 and 16.
 *
 * The target keeps the instruction record address in $s1 across the wldcore_bits_test call instead of
 * rebuilding it at each byte load. GCC folds a symbol address straight into both `lbu`s, so the empty
 * tied `__asm__` (which emits no instruction) is what forces the base into its own pseudo; no plain C
 * spelling of the two loads reproduces it.
 *
 * The target frame reserves 8 bytes of locals that no instruction references; frame_reservation
 * reproduces that reservation and the resulting 0x18/0x1c/0x20 saved-register offsets. The original
 * local it stood for is unknown. */
void wldcore_sound_novel_branch_if_section_read_bit(s32 expected) {
    wldcore_sound_novel_instruction_bytes_t* instruction = &g_wldcore_sound_novel_state.instruction.bytes;
    u16 flags;
    s32 frame_reservation[2];

    __asm__("" : "=r"(instruction) : "0"(instruction));

    if (wldcore_bits_test(g_main_sound_novel_section_bits, (instruction->operand_0 << 8) | instruction->operand_1)
        == expected) {
        flags = g_wldcore_sound_novel_record.state_flags | WLDCORE_SOUND_NOVEL_STATE_BRANCH;
        g_wldcore_sound_novel_record.section = instruction->operand_2;
    } else {
        flags = g_wldcore_sound_novel_record.state_flags | WLDCORE_SOUND_NOVEL_STATE_FETCH;
    }
    g_wldcore_sound_novel_record.state_flags = flags;
}
