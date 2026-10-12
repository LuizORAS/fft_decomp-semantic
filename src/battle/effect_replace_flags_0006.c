#include "fft/battle_effect.h"

/* The two flag meanings remain unknown. */
enum { EFFECT_REPLACED_FLAGS_MASK = 0x0006 };

/* Nothing in the game calls it. */
void effect_replace_flags_0006(effect_flag_prefix_t* object, s32 value) {
    object->flags = (object->flags & ~EFFECT_REPLACED_FLAGS_MASK) | (value & EFFECT_REPLACED_FLAGS_MASK);
}
