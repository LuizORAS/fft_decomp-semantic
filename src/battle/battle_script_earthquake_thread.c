#include "fft/battle_camera.h"
#include "fft/battle_script.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Thread body of EarthquakeStart (EVENT_OPCODE_EARTHQUAKE_START), task THREAD_TASK_EARTHQUAKE:
 * every operand-1 frames add operand 0 to the camera shake offset (g_battle_camera_shake_z_offset), and
 * every operand-3 frames add operand 2. It never ends by itself: EarthquakeEnd suspends the task's
 * thread, so the emitted epilogue is unreachable. */
void battle_script_earthquake_thread(void) {
    event_earthquake_parameters_t* parameters;
    s32 first_counter;
    s32 second_counter;

    thread_set_current_task_id(THREAD_TASK_EARTHQUAKE);
    parameters = (event_earthquake_parameters_t*)battle_thread_get_current_parameter_1();
    first_counter = 0;
    second_counter = 0;
    for (;;) {
        thread_yield();
        first_counter++;
        if (first_counter == parameters->pulses[0].frames) {
            g_battle_camera_shake_z_offset += parameters->pulses[0].z_delta;
            first_counter = 0;
        }
        second_counter++;
        if (second_counter == parameters->pulses[1].frames) {
            g_battle_camera_shake_z_offset += parameters->pulses[1].z_delta;
            second_counter = 0;
        }
    }
}
