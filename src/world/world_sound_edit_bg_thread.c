#include "fft/world.h"
#include "psx/types.h"

/* Thread task 0x35 continuation: ramp the parameter sound's volume from
 * parameter[1] to parameter[2] over parameter[4] yields, then settle on the
 * final level and exit the thread. */
void world_sound_edit_bg_thread(void) {
    u8* parameter;
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
    sound_id = parameter[0];
    volume_from = parameter[1];
    steps = parameter[4];
    volume_to = parameter[2];
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
