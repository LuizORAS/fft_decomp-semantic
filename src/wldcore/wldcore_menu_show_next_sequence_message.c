#include "fft/wldcore.h"
#include "psx/types.h"

void wldcore_menu_show_next_sequence_message(wldcore_menu_countdown_t* countdown) {
    world_thread_set_parameters(14, 0x19, countdown->current, 0);
    countdown->current++;
    countdown->remaining--;
    if (countdown->remaining == 0) {
        wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_OUT, 4);
        countdown->phase = WLDCORE_COUNTDOWN_PHASE_LAST_MESSAGE;
        countdown->timer = 0;
    } else {
        countdown->phase = WLDCORE_COUNTDOWN_PHASE_MESSAGES;
    }
}
