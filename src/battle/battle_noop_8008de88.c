#include "fft/battle.h"

/* An empty routine between battle_state_enter_effect_playback and battle_state_clear_slowdown. Nothing on
 * the disc calls it or holds its address. */
void battle_noop_8008de88(void) {
}
