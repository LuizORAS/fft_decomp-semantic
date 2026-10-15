#include "fft/wldcore.h"

/* Twin of wldcore_file_wait_for_load with the audio-queue depth
 * as the wait condition. The two busy words are read as g_wldcore_audio_queue members
 * rather than as separate scalars. */
void wldcore_sound_wait_for_queue_drain(void) {
    while (g_wldcore_audio_queue.count != 0 || g_wldcore_audio_queue.current_command != WLDCORE_AUDIO_COMMAND_NONE) {
        main_file_poll_load(&g_main_file_cd_state);
        wldcore_sound_process_audio_queue();
        VSync(0);
    }
}
