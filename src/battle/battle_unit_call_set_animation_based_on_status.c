#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_unit_set_animation_based_on_status; the learn-on-hit and resume-attack states call
 * it. */
void battle_unit_call_set_animation_based_on_status(battle_unit_misc_data_t* unit) {
    battle_unit_set_animation_based_on_status(unit);
}
