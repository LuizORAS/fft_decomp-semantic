#include "fft/battle.h"

/* Same as battle_unit_clear_distortion_animation_id_10, for distortion 0xb (BATTLE_DISTORTION_STOP_2,
 * queued by no SEQ file). */
void battle_unit_clear_distortion_animation_id_11(battle_unit_misc_data_t* unit) {
    unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
}
