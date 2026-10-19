#include "fft/battle_effect.h"

/* Set g_battle_effect_gravity, the acceleration battle_effect_step_motion and
 * battle_effect_integrate_particle_motion add to each velocity, scaled by the particle's weight. Script opcode
 * 27 takes it from the effect file; the secondary-effect handlers set (0, ONE, 0) while they step their
 * slots. */
void battle_effect_set_gravity(const battle_effect_gravity_t* src) {
    g_battle_effect_gravity = *src;
}
