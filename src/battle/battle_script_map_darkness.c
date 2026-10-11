#include "fft/battle.h"
#include "psx/types.h"

/* Thread body of MapDarkness: start the ambient-light blend described by the
 * operands (blend mode, red, green, blue as s8, duration index), then keep the
 * thread alive under the MapDarkness task id for
 * g_battle_script_map_darkness_durations[duration index] frames (1 to 256). */
void battle_script_map_darkness(void) {
    u8* parameters;

    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_MAP_DARKNESS);
    parameters = (u8*)battle_thread_get_current_parameter_1();
    battle_map_start_ambient_light_blend(
        parameters[0], parameters[4], (s8)parameters[1], (s8)parameters[2], (s8)parameters[3]);
    battle_thread_wait_frames(g_battle_script_map_darkness_durations[parameters[4]]);
    battle_thread_exit_current();
}
