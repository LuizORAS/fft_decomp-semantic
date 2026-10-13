#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Two independent frame counters apply the two shake pulses until another
 * instruction tears the task down. */
void world_script_earthquake_start(void) {
    s32 first_frames;
    s32 second_frames;
    event_earthquake_parameters_t* parameter;

    thread_set_current_task_id(THREAD_TASK_EARTHQUAKE);
    first_frames = 0;
    parameter = world_thread_get_current_parameter_1();
    second_frames = 0;
    for (;;) {
        first_frames++;
        thread_yield();
        second_frames++;
        if (first_frames == parameter->pulses[0].frames) {
            first_frames = 0;
            g_world_camera_shake_z_offset += parameter->pulses[0].z_delta;
        }
        if (second_frames == parameter->pulses[1].frames) {
            g_world_camera_shake_z_offset += parameter->pulses[1].z_delta;
            second_frames = 0;
        }
    }
}
