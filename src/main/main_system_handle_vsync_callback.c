#include "fft/main.h"

/* VSync callback: advance rand once per frame, count the play time (hours stop at 1000) and the
 * free-running frame timer. */
void main_system_handle_vsync_callback(void) {
    rand();

    g_main_system_play_time_frames++;
    if (g_main_system_play_time_frames >= 60) {
        g_main_system_play_time_frames = 0;
        g_main_system_play_time_seconds++;
        if (g_main_system_play_time_seconds >= 60) {
            g_main_system_play_time_seconds = 0;
            g_main_system_play_time_minutes++;
            if (g_main_system_play_time_minutes >= 60) {
                g_main_system_play_time_minutes = 0;
                if (g_main_system_play_time_hours < 1000) {
                    g_main_system_play_time_hours++;
                }
            }
        }
    }

    g_main_system_frame_timer++;
}
