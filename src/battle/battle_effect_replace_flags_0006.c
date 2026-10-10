#include "fft/battle.h"

/* Nothing in the game calls it. */
void battle_effect_replace_flags_0006(battle_effect_flag_prefix_t* object, s32 value) {
    object->flags = (object->flags & ~BATTLE_EFFECT_FLAGS_0006) | (value & BATTLE_EFFECT_FLAGS_0006);
}
