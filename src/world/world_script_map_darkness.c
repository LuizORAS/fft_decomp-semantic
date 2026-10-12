#include "fft/battle_map.h"
#include "fft/world.h"

/* Thread body of the map darkness event instruction: start the ambient
 * light blend from the five parameter bytes (mode, signed RGB amounts,
 * speed) and wait the number of frames the table gives for that speed. */
void world_script_map_darkness(void) {
    event_unit_color_t* parameter;

    world_thread_set_current_task_id(NATIVE_THREAD_TASK_MAP_DARKNESS);
    parameter = world_thread_get_current_parameter_1();
    battle_map_start_ambient_light_blend(
        parameter->mode, parameter->duration, parameter->rgb.r, parameter->rgb.g, parameter->rgb.b);
    world_thread_wait_frames(g_world_map_darkness_wait_frames[parameter->duration]);
    world_thread_exit_current();
}
