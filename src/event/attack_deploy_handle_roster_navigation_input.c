#include "fft/event_attack.h"
#include "psx/pad.h"
#include "psx/types.h"

/* L1 and R1 on the deployment screen: start the previous- or next-unit
 * thread in the roster step slot, unless one is still running. */
void attack_deploy_handle_roster_navigation_input(void) {
    u32* flags;

    flags = script_get_controller_input_pointer(0);
    if ((*flags & PSX_PAD_L1) && (battle_thread_is_running(ATTACK_THREAD_SLOT_ROSTER_STEP) == 0)) {
        g_attack_panel_status_animation[0] = 7;
        g_sound_pending_sfx_id = MAIN_SFX_PAGE_SWITCH;
        battle_thread_start(ATTACK_THREAD_SLOT_ROSTER_STEP, attack_deploy_select_previous_roster_unit);
    }
    if ((*flags & PSX_PAD_R1) && (battle_thread_is_running(ATTACK_THREAD_SLOT_ROSTER_STEP) == 0)) {
        g_attack_panel_status_animation[1] = 7;
        g_sound_pending_sfx_id = MAIN_SFX_PAGE_SWITCH;
        battle_thread_start(ATTACK_THREAD_SLOT_ROSTER_STEP, attack_deploy_select_next_roster_unit);
    }
}
