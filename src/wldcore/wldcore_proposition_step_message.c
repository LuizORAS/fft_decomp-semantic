#include "fft/script.h"
#include "fft/thread.h"
#include "fft/wldcore.h"
#include "psx/pad.h"

/*
 * Queue a proposition message and wait for its acknowledgment.
 *
 * Return 1 and clear the caller's phase after a new button-0x20 press
 * with the menu event flag set. Initial setup and continued waiting return 0.
 */
s32 wldcore_proposition_step_message(wldcore_proposition_report_level_t* state, s32 message) {
    if (state->phase == 0) {
        s32* party_index;
        state->phase = 1;
        if (!thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE))
            thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
        party_index = &g_wldcore_proposition_report.excluded_party_index;
        g_text_substitution_values[0] = *party_index + TEXT_ID_UNIT_NAME_BASE;
        script_set_variable(EVENT_SCRIPT_VAR_DIALOG_PORTRAIT, world_get_party_unit_formation_sprite(*party_index));
        thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19,
            wldcore_proposition_adjust_message_index(message, *party_index) + TEXT_ID_WORLD_MESSAGE_BASE, 0);
    } else if (g_wldcore_input_new_button_presses & PSX_PAD_CIRCLE) {
        if (world_menu_get_event_state_flag()) {
            state->phase = 0;
            return 1;
        }
    }
    return 0;
}
