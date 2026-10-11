#include "fft/world.h"
#include "psx/types.h"

/* Ramp the environment sound from volume_from to volume_to over frames yields,
 * then settle on the final level and exit the thread. */
void world_sound_edit_bg_thread(void) {
    event_background_sound_parameters_t* parameter;
    s32 sound_id;
    s32 volume_from;
    s32 volume_to;
    s32 steps;
    s32 delta;
    s32 accumulator;
    s32 step;
    s32 volume;
    s32 resource;
    u8 unused[8]; /* retail frame reserves eight bytes of locals */

    world_thread_set_current_task_id(NATIVE_THREAD_TASK_BG_SOUND);
    step = 0;
    parameter = world_thread_get_current_parameter_1();
    sound_id = parameter->sound_id;
    volume_from = parameter->volume_from;
    steps = parameter->frames;
    volume_to = parameter->volume_to;
    resource = MAIN_SOUND_ID(MAIN_SOUND_RESOURCE_ENV, 0);
    if (steps != 0) {
        delta = volume_to - volume_from;
        accumulator = 0;
        do {
            world_thread_yield();
            volume = (accumulator / steps) + volume_from;
            if (volume < 0) {
                volume = -volume;
            }
            if (volume == 0) {
                volume = 1;
            }
            accumulator += delta;
            main_sound_set_sfx_echo(sound_id + resource, volume);
            step++;
        } while (step < steps);
    }
    main_sound_set_sfx_echo(sound_id | resource, volume_to);
    world_thread_exit_current();
}
