#include "fft/battle.h"

/* Run the unit's SEQ-queued motion for this frame: the handler in
 * g_battle_unit_distortion_animation_handlers that distortion_animation_id selects (BATTLE_DISTORTION_*).
 * Id 0 is none; ids from BATTLE_DISTORTION_ANIMATION_COUNT report animation error 0x17.
 * battle_gfx_update_and_animate_unit_wep_eff calls it after the unit's SEQ script, where opcode 0xc1
 * queues a motion and its frames and 0xc0 waits until it ends. */
/* Step the unit's distortion animation, if it has one: a motion that a
 * sequence started with BATTLE_SEQ_OP_START_DISTORTION and that its handler
 * ends by clearing the id. */
void battle_unit_dispatch_distortion_animation(battle_unit_misc_data_t* unit) {
    u32 distortion_id = unit->distortion_animation_id;

    if (distortion_id != BATTLE_DISTORTION_ANIMATION_NONE) {
        if (distortion_id < BATTLE_DISTORTION_ANIMATION_COUNT) {
            g_battle_unit_distortion_animation_handlers[distortion_id](unit);
        } else {
            main_system_handle_error(MAIN_SYSTEM_ERROR_DISTORTION_ANIMATION);
        }
    }
}
