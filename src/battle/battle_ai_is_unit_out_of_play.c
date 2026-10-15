#include "fft/battle.h"
#include "psx/types.h"

/* Return 1 when the AI ignores unit_id: untargetable, dead without Reraise, or petrified. */
s32 battle_ai_is_unit_out_of_play(s32 unit_id) {
    u8 flags = g_battle_ai_data_base.unit_decisions[unit_id].targeting_flags_2;

    if (((flags >> 4) & (BATTLE_AI_TARGET_UNTARGETABLE >> 4))
        || ((flags >> 5) & (BATTLE_AI_TARGET_DEAD_WITHOUT_RERAISE >> 5))) {
        return 1;
    }
    return g_battle_unit_stats[unit_id].status_sets.current[1] >> 7;
}
