#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 34: branch to the section in the top operand byte when the counter plus its
 * pending delta, clamped to 0..0xFFFF, equals the 16-bit operand (instruction bits 8..23).
 *
 * The target's sum is `addu v1,v0,v1`: rs is the 32-bit g_wldcore_sound_novel_record.counter, rt the
 * sign-extended counter_delta 16-bit value. Any `+` spelling canonicalises the 16-bit value into rs.
 * `a - (-b)` reaches fold as MINUS_EXPR(a, NEGATE_EXPR(b)) and is rewritten to a PLUS_EXPR without the
 * commutative re-canonicalisation, which keeps the target's operand order. */
void wldcore_sound_novel_branch_if_counter_equal(void) {
    s32 value;
    s32 operand;
    u32 word;

    value = g_wldcore_sound_novel_record.counter - (-g_wldcore_sound_novel_record.counter_delta);
    if (0xffff < value) {
        value = 0xffff;
    }
    if (value < 0) {
        value = 0;
    }
    word = g_wldcore_sound_novel_state.instruction.word;
    operand = (word >> 8) & 0xffff;
    if (value == operand) {
        g_wldcore_sound_novel_record.section = word >> 24;
        g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_BRANCH;
    } else {
        g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
    }
}
