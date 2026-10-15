#include "fft/wldcore.h"
#include "psx/pad.h"

/* Step of the discovery milestone: Circle shows the next message; after the last one, once the text
 * is done, the milestone jingle (MAIN_MUSIC_MILESTONE) plays for at least 180 frames, the Bar's music
 * returns, and once the audio queue is idle the level closes into the list of job reports.
 *
 * The busy words are read as g_wldcore_audio_queue members, not scalars, which
 * keeps the timer store ahead of their loads as in the target. */
void wldcore_proposition_step_milestone_level(wldcore_menu_milestone_level_t* milestone) {
    if (milestone->phase == WLDCORE_MILESTONE_PHASE_CLOSE) {
        if (g_wldcore_audio_queue.count == 0 && g_wldcore_audio_queue.current_command == WLDCORE_AUDIO_COMMAND_NONE) {
            g_wldcore_menu_stack_depth--;
            wldcore_proposition_open_report_list();
        }
    } else if (milestone->phase == WLDCORE_MILESTONE_PHASE_JINGLE) {
        milestone->timer++;
        if (g_wldcore_audio_queue.count == 0 && g_wldcore_audio_queue.current_command == WLDCORE_AUDIO_COMMAND_NONE
            && milestone->timer >= 180) {
            milestone->phase = WLDCORE_MILESTONE_PHASE_CLOSE;
            wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_OUT, 0x10);
            wldcore_sound_enqueue_audio_command(
                WLDCORE_AUDIO_COMMAND_PLAY_MUSIC, WLDCORE_AUDIO_MUSIC(2, MAIN_MUSIC_BAR));
            wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_IN, 0x10);
        }
    } else if (milestone->phase == WLDCORE_MILESTONE_PHASE_LAST_MESSAGE) {
        if (world_menu_is_message_waiting_for_confirm() != 0
            && g_wldcore_audio_queue.current_command == WLDCORE_AUDIO_COMMAND_NONE
            && g_wldcore_audio_queue.count == 0) {
            wldcore_sound_enqueue_audio_command(
                WLDCORE_AUDIO_COMMAND_PLAY_MUSIC, WLDCORE_AUDIO_MUSIC(2, MAIN_MUSIC_MILESTONE));
            wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_IN, 0x10);
            milestone->phase = WLDCORE_MILESTONE_PHASE_JINGLE;
        }
    } else if (g_wldcore_new_button_presses & PSX_PAD_CIRCLE) {
        wldcore_proposition_show_next_milestone_message(milestone);
    }
}
