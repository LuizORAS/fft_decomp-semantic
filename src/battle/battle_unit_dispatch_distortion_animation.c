#include "fft/battle_unit.h"

/* Run the unit's SEQ-queued motion for this frame: the handler in
 * g_battle_unit_distortion_animation_handlers that distortion_animation_id selects (BATTLE_DISTORTION_ANIMATION_*),
 * which ends the motion by clearing the id. Id 0 is none; ids from BATTLE_DISTORTION_ANIMATION_COUNT
 * report animation error 0x17. battle_gfx_update_and_animate_unit_wep_eff calls it after the unit's SEQ
 * script, where opcode 0xc1 queues a motion and its frames and 0xc0 waits until it ends. */
void battle_unit_dispatch_distortion_animation(battle_unit_t* unit) {
    u32 distortion_id = unit->distortion_animation_id;

    if (distortion_id != BATTLE_DISTORTION_ANIMATION_NONE) {
        if (distortion_id < BATTLE_DISTORTION_ANIMATION_COUNT) {
            g_battle_unit_distortion_animation_handlers[distortion_id](unit);
        } else {
            main_system_handle_error(MAIN_SYSTEM_ERROR_DISTORTION_ANIMATION);
        }
    }
}
