#include "fft/world.h"
#include "psx/types.h"

/* Thread task 0x35: restart the parameter's sound of sound resource 1 (ENV.SED),
 * apply its volume, then continue in the looping editor thread. */
void world_sound_bg_thread(void) {
    event_background_sound_parameters_t* parameter;
    s32 sound_id;
    s32 handle;
    s32 volume;
    s32 resource;

    world_thread_set_current_task_id(NATIVE_THREAD_TASK_BG_SOUND);
    parameter = world_thread_get_current_parameter_1();
    sound_id = parameter->sound_id;
    resource = MAIN_SOUND_ID(MAIN_SOUND_RESOURCE_ENV, 0);
    if (parameter->find_channel != 0) {
        handle = sound_id + resource;
        main_sound_stop_sfx(handle);
        main_sound_play_sfx_find_channel(handle);
    } else {
        handle = sound_id + resource;
        main_sound_stop_sfx(handle);
        main_sound_play_bg_sfx(handle);
    }
    volume = parameter->volume_from;
    if (volume == 0) {
        volume = 1;
    }
    main_sound_set_sfx_echo(sound_id | resource, volume);
    world_sound_edit_bg_thread();
}
