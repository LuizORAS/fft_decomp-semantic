#include "fft/battle.h"
#include "psx/types.h"

/* Thread body of EarthquakeStart (EVENT_OPCODE_EARTHQUAKE_START), task NATIVE_THREAD_TASK_EARTHQUAKE:
 * every operand-1 frames add operand 0 to the camera shake offset (g_battle_camera_shake_z_offset), and
 * every operand-3 frames add operand 2. It never ends by itself: EarthquakeEnd suspends the task's
 * thread, so the emitted epilogue is unreachable. */
void battle_script_earthquake_thread(void) {
    u8* parameters;
    s32 first_counter;
    s32 second_counter;

    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_EARTHQUAKE);
    parameters = (u8*)battle_thread_get_current_parameter_1();
    first_counter = 0;
    second_counter = 0;
    for (;;) {
        battle_thread_yield();
        first_counter++;
        if (first_counter == parameters[1]) {
            g_battle_camera_shake_z_offset += parameters[0];
            first_counter = 0;
        }
        second_counter++;
        if (second_counter == parameters[3]) {
            g_battle_camera_shake_z_offset += parameters[2];
            second_counter = 0;
        }
    }
}
