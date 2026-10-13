#include "fft/battle_map.h"
#include "fft/battle_script.h"
#include "fft/battle_thread.h"
#include "fft/thread.h"

/* Thread body of MapDarkness: start the ambient-light blend described by the
 * operands (blend mode, red, green, blue as s8, duration index), then keep the
 * thread alive under the MapDarkness task id for
 * g_script_map_darkness_durations[duration index] frames (1 to 256). */
void script_map_darkness(void) {
    event_unit_color_t* parameters;

    thread_set_current_task_id(NATIVE_THREAD_TASK_MAP_DARKNESS);
    parameters = (event_unit_color_t*)battle_thread_get_current_parameter_1();
    map_start_ambient_light_blend(
        parameters->mode, parameters->duration, parameters->rgb.r, parameters->rgb.g, parameters->rgb.b);
    thread_wait_frames(g_script_map_darkness_durations[parameters->duration]);
    thread_exit_current();
}
