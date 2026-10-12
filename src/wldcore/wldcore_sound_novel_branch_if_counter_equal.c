#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 34: branch to the section in the top operand byte when the counter plus its
 * pending delta, clamped to 0..0xFFFF, equals the 16-bit operand (instruction bits 8..23).
 *
 * The target's sum is `addu v1,v0,v1`: rs is the g_wldcore_sound_novel_record.counter word, rt the
 * sign-extended counter_delta halfword. Any `+` spelling canonicalises the halfword into rs.
 * `a - (-b)` reaches fold as MINUS_EXPR(a, NEGATE_EXPR(b)) and is rewritten to a PLUS_EXPR without the
 * commutative re-canonicalisation, which keeps the target's operand order. */
void wldcore_sound_novel_branch_if_counter_equal(void) {
    s32 value;
    s32 operand;
    u32 word;
    u16 flags;

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
        flags = g_wldcore_sound_novel_record.state_flags;
        g_wldcore_sound_novel_record.section = word >> 24;
        g_wldcore_sound_novel_record.state_flags = flags | WLDCORE_SOUND_NOVEL_STATE_BRANCH;
    } else {
        g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
    }
}
