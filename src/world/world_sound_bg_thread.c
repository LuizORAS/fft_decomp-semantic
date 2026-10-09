#include "fft/world.h"
#include "psx/types.h"

/* Thread task 0x35: restart the parameter's sound of sound resource 1 (ENV.SED),
 * apply its echo setting, then continue in the looping editor thread. */
void world_sound_bg_thread(void) {
    u8* parameter;
    s32 sound_id;
    s32 handle;
    s32 echo;
    s32 resource;
    s32 use_find_channel;

    world_thread_set_current_task_id(NATIVE_THREAD_TASK_BG_SOUND);
    parameter = world_thread_get_current_parameter_1();
    use_find_channel = parameter[3];
    sound_id = parameter[0];
    resource = 0x10000;
    if (use_find_channel != 0) {
        handle = sound_id + resource;
        main_sound_stop_sfx(handle);
        main_sound_play_sfx_find_channel(handle);
    } else {
        handle = sound_id + resource;
        main_sound_stop_sfx(handle);
        main_sound_play_bg_sfx(handle);
    }
    echo = parameter[1];
    if (echo == 0) {
        echo = 1;
    }
    main_sound_set_sfx_echo(sound_id | resource, echo);
    world_sound_edit_bg_thread();
}
