#include "fft/battle_unit.h"

/* Distortion 0xa (BATTLE_DISTORTION_ANIMATION_CLEAR, queued by no SEQ file): end the queued motion at once;
 * battle_unit_clear_distortion_animation_id_11 does the same for 0xb. */
void battle_unit_clear_distortion_animation_id_10(battle_unit_t* unit) {
    unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
}
