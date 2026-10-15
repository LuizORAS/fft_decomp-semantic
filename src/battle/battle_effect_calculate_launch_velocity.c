#include "fft/battle.h"
#include "psx/types.h"

/* Return the launch speed of an arcing projectile in 20.12: the square root of
 * g_battle_effect_gravity_modifier (168.0) times g_battle_effect_gravity_scale (0x53a, about 0.33), about 7.4.
 * Nothing writes either constant. The arrow arc handler and battle_effect_trace_arc_trajectory_path call it. */
s32 battle_effect_calculate_launch_velocity(void) {
    return SquareRoot12((g_battle_effect_gravity_modifier * g_battle_effect_gravity_scale) >> 12);
}
