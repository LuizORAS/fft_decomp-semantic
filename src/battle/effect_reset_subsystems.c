#include "fft/battle_effect.h"

/* Four effect-pool/state initializers, called at effect setup and teardown. */
void effect_reset_subsystems(void) {
    effect_init_render_state();
    effect_init_state_records();
    effect_init_record_chain();
    effect_init_free_list();
}
