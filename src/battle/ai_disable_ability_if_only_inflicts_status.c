#include "fft/battle_ai.h"

/*
 * Disable an AI ability that does nothing except add one unwanted status.
 *
 * The 16-bit value update spans skillset and usage_flags so GCC emits the target's
 * single lhu/andi/sh sequence while clearing only the usable bit. A direct
 * status_infliction pointer folds +7 into its base; byte-stepping the typed AI
 * container retains the target's base pointer and named field access.
 */
void ai_disable_ability_if_only_inflicts_status(ai_ability_entry_t* entry, s32 status_id) {
    ai_data_t* ai = &g_ai_data_base;

    if ((ai->considered_ability.ai_flags.word & AI_ABILITY_ADD_STATUS) == 0) {
        return;
    }
    /* Only ally targeting and status addition may remain in this flag byte. */
    if ((ai->considered_ability.ai_flags.bytes.flags_1
            & (AI_ABILITY_TARGET_ENEMIES | AI_ABILITY_UNEQUIP | AI_ABILITY_AFFECTS_STATS | AI_ABILITY_CANCEL_STATUS
                | AI_ABILITY_AFFECTS_MP | AI_ABILITY_AFFECTS_HP))
        != 0) {
        return;
    }

    {
        s32 status_set;
        s32 mask;
        ai_data_t* cursor;
        s32 shift;

        status_set = 0;
        mask = 0x80;
        cursor = &g_ai_data_base;
        shift = status_id;
        for (; status_set < STATUS_BYTE_COUNT; status_set++, shift -= 8) {
            if (status_set == status_id / 8) {
                if ((cursor->considered_ability.parameters.bytes.status_infliction[0] & ((mask >> shift) ^ 0xFF))
                    != 0) {
                    return;
                }
                /* A separate increment per branch keeps the target's cursor
                 * base; one shared increment folds the field offset into it. */
                cursor = (ai_data_t*)((u8*)cursor + 1);
                continue;
            }
            if (cursor->considered_ability.parameters.bytes.status_infliction[0] != 0) {
                return;
            }
            cursor = (ai_data_t*)((u8*)cursor + 1);
        }
    }
    entry->usage.policy.usable = 0;
}
