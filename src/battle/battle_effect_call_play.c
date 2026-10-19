#include "fft/battle_effect.h"

/* Call battle_effect_play: script_play_effect_and_wait runs it on the main stack for the
 * UnitAnim instruction after a bow animation. */
void battle_effect_call_play(void) {
    battle_effect_play();
}
