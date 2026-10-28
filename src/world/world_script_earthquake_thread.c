#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Thread body of EarthquakeStart (EVENT_OPCODE_EARTHQUAKE_START): every operand-1 frames add operand
 * 0 to g_world_camera_shake_z_offset, and every operand-3 frames add operand 2. */
void world_script_earthquake_thread(void) {
    s32 first_frames;
    s32 second_frames;
    event_earthquake_parameters_t* parameter;

    thread_set_current_task_id(THREAD_TASK_EARTHQUAKE);
    first_frames = 0;
    parameter = (event_earthquake_parameters_t*)world_thread_get_current_parameter_1();
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
