#include "fft/wldcore.h"
#include "psx/types.h"

/* Branch to section operand 2 when the read flag of section operand 1 of novel operand 0
 * (g_main_sound_novel_section_bits, set as a section is entered) equals `expected`; otherwise go on.
 * Opcodes 15 and 16. */
void wldcore_sound_novel_branch_if_section_read_bit(s32 expected) {
    u32* instruction = &g_wldcore_sound_novel_state.instruction.word;
    u16 flags;

    if (wldcore_bits_test(g_main_sound_novel_section_bits, ((u8)(*instruction >> 8) << 8) | (u8)(*instruction >> 16))
        == expected) {
        flags = g_wldcore_sound_novel_record.state_flags | WLDCORE_SOUND_NOVEL_STATE_BRANCH;
        g_wldcore_sound_novel_record.section = (u8)(*instruction >> 24);
    } else {
        flags = g_wldcore_sound_novel_record.state_flags | WLDCORE_SOUND_NOVEL_STATE_FETCH;
    }
    g_wldcore_sound_novel_record.state_flags = flags;
}
