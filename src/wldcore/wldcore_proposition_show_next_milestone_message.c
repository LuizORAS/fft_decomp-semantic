#include "fft/wldcore.h"
#include "psx/types.h"

/* Show the milestone's next message and count it off; after the last one the music fades out and
 * the level moves to WLDCORE_MILESTONE_PHASE_LAST_MESSAGE with its timer cleared. */
void wldcore_proposition_show_next_milestone_message(wldcore_menu_milestone_level_t* milestone) {
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, milestone->current, 0);
    milestone->current++;
    milestone->remaining--;
    if (milestone->remaining == 0) {
        wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_OUT, 4);
        milestone->phase = WLDCORE_MILESTONE_PHASE_LAST_MESSAGE;
        milestone->timer = 0;
    } else {
        milestone->phase = WLDCORE_MILESTONE_PHASE_MESSAGES;
    }
}
