#include "fft/battle_map.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Thread body of the map darkness event instruction: start the ambient
 * light blend from the five parameter bytes (mode, signed RGB amounts,
 * speed) and wait the number of frames the table gives for that speed. */
void world_script_map_darkness_thread(void) {
    event_unit_color_t* parameter;

    thread_set_current_task_id(THREAD_TASK_MAP_DARKNESS);
    parameter = (event_unit_color_t*)world_thread_get_current_parameter_1();
    battle_map_start_ambient_light_blend(
        parameter->mode, parameter->duration, parameter->rgb.r, parameter->rgb.g, parameter->rgb.b);
    thread_wait_frames(g_world_script_map_darkness_durations[parameter->duration]);
    thread_exit_current();
}
