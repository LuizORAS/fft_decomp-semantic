#include "fft/wldcore.h"
#include "psx/pad.h"

/* Show report message `message` in the reporting unit's voice
 * (wldcore_proposition_adjust_message_index), with the unit's name and portrait, starting the message
 * box if needed. Returns 1, resetting the phase, once Circle is pressed after the text has finished,
 * else 0. */
s32 wldcore_proposition_step_message(wldcore_proposition_report_level_t* state, s32 message) {
    if (state->phase == 0) {
        s32* party_index;
        state->phase = 1;
        if (!world_thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE))
            world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
        party_index = &g_wldcore_proposition_outcome.reporter_party_index;
        g_world_text_substitution_values[0] = *party_index + TEXT_ID_UNIT_NAME_SPECIAL_BASE;
        world_script_set_variable(
            EVENT_SCRIPT_VAR_DIALOG_PORTRAIT, world_get_party_unit_formation_sprite(*party_index));
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19,
            wldcore_proposition_adjust_message_index(message, *party_index) + TEXT_ID_WORLD_MESSAGE_BASE, 0);
    } else if (g_wldcore_new_button_presses & PSX_PAD_CIRCLE) {
        if (world_menu_is_message_waiting_for_confirm()) {
            state->phase = 0;
            return 1;
        }
    }
    return 0;
}
