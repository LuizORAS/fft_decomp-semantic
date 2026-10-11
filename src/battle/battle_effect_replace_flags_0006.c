#include "fft/battle.h"

/* The two flag meanings remain unknown. */
enum { BATTLE_EFFECT_REPLACED_FLAGS_MASK = 0x0006 };

/* Nothing in the game calls it. */
void battle_effect_replace_flags_0006(battle_effect_flag_prefix_t* object, s32 value) {
    object->flags = (object->flags & ~BATTLE_EFFECT_REPLACED_FLAGS_MASK) | (value & BATTLE_EFFECT_REPLACED_FLAGS_MASK);
}
