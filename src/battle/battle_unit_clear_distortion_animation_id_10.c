#include "fft/battle.h"

/* Distortion 0xa (BATTLE_DISTORTION_STOP, queued by no SEQ file): end the queued motion at
 * once. */
void battle_unit_clear_distortion_animation_id_10(battle_unit_misc_data_t* unit) {
    unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
}
