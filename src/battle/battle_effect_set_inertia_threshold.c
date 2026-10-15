#include "fft/battle.h"

/* Set g_battle_effect_inertia_threshold: each frame a particle keeps (inertia - threshold) / inertia of
 * its velocity. Script opcode 27 and the secondary-effect handlers set it as they set the gravity. */
void battle_effect_set_inertia_threshold(s32 threshold) {
    g_battle_effect_inertia_threshold = threshold;
}
