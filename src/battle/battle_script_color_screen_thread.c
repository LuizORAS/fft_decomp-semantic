#include "fft/battle.h"
#include "psx/types.h"

/* Fades the screen colour modulation from a start RGB to an end RGB over a duration.
 *
 * Twin of world_script_color_screen_thread. Parameters: mode byte, start RGB, end RGB,
 * then a halfword duration in frames. The colour advances by two frames per
 * step and finally snaps to the end colour. */
void battle_script_color_screen_thread(void) {
    union {
        struct {
            s32 start[3];
            s32 end[3];
            s32 current[3];
        } rgb;
        s32 values[9];
    } color;
    u8* parameters;
    s32 mode;
    s32 i;
    s32 j;
    s32 duration;

    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_COLOR_SCREEN);
    parameters = (u8*)(event_color_screen_parameters_t*)battle_thread_get_current_parameter_1();
    /* Separate record and RGB cursors drop the original argument-register copy. */
    mode = ((event_color_screen_parameters_t*)parameters++)->mode;
    for (i = 0; i < 6; i++) {
        color.values[i] = *parameters++;
    }
    /* The RGB scan leaves the cursor at duration_le. */
    duration = battle_script_load_halfword(parameters);
    for (i = 0; i < duration; i += 2) {
        battle_thread_yield();
        battle_thread_yield();
        for (j = 0; j < 3; j++) {
            color.rgb.current[j] = (color.rgb.end[j] - color.rgb.start[j]) * i / duration + color.rgb.start[j];
        }
        battle_gfx_start_screen_color_modulation_fade(
            mode, color.rgb.current[0], color.rgb.current[1], color.rgb.current[2], 2);
    }
    for (j = 0; j < 3; j++) {
        color.rgb.current[j] = color.rgb.end[j];
    }
    battle_gfx_start_screen_color_modulation_fade(
        mode, color.rgb.current[0], color.rgb.current[1], color.rgb.current[2], 2);
    battle_thread_yield();
    battle_thread_yield();
    battle_thread_exit_current();
}
