#include "fft/battle.h"
#include "fft/battle_gfx.h"
#include "fft/battle_script.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Fades the screen colour modulation from a start RGB to an end RGB over a duration.
 *
 * Twin of world_script_color_screen_thread. Parameters: mode byte, start RGB, end RGB,
 * then a 16-bit duration in frames. The colour advances by two frames per
 * step and finally snaps to the end colour. */
void script_color_screen_thread(void) {
    struct {
        s32 start_and_end[6];
        s32 current[3];
    } color;
    u8* parameters;
    s32 mode;
    s32 i;
    s32 j;
    s32 duration;

    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_COLOR_SCREEN);
    parameters = (u8*)battle_thread_get_current_parameter_1();
    /* Separate record and RGB cursors drop the original argument-register copy. */
    mode = *parameters++;
    for (i = 0; i < 6; i++) {
        color.start_and_end[i] = *parameters++;
    }
    /* The RGB scan leaves the cursor at duration_le. */
    duration = script_load_halfword(parameters);
    for (i = 0; i < duration; i += 2) {
        battle_thread_yield();
        battle_thread_yield();
        for (j = 0; j < 3; j++) {
            s32 end = color.start_and_end[j + 3];
            s32 start = color.start_and_end[j];
            color.current[j] = (end - start) * i / duration + start;
        }
        battle_gfx_start_screen_color_modulation_fade(mode, color.current[0], color.current[1], color.current[2], 2);
    }
    for (j = 0; j < 3; j++) {
        color.current[j] = color.start_and_end[j + 3];
    }
    battle_gfx_start_screen_color_modulation_fade(mode, color.current[0], color.current[1], color.current[2], 2);
    battle_thread_yield();
    battle_thread_yield();
    battle_thread_exit_current();
}
