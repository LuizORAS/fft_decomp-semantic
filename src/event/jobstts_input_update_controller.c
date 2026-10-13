#include "fft/event_jobstts.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Read pad 0 and derive JOBSTTS's primary/secondary auto-repeat input words.
 *
 * Port of card_input_update_controller with the event
 * speed fixed at 2; the divisor stays a variable, which is why the target
 * divides with divu rather than shifting. */
void jobstts_input_update_controller(void) {
    s16 bit;
    s32 index;
    /* Pinned: the copies a hard-register variable costs keep the third loop above
     * loop.c's size threshold, so the `index == 4` constant is reloaded each pass. */
    register u32 current_input __asm__("$8");
    u32 filtered_input;
    s32* counter;
    s32* counter_base;
    u32 speed, initial_delay, secondary_repeat_period, repeat_period;
    speed = 2;
    initial_delay = g_main_input_repeat_initial_delay / speed;
    repeat_period = g_main_input_repeat_period / speed;
    secondary_repeat_period = g_main_input_secondary_repeat_period / speed;
    current_input = PadRead(0);
    if (current_input & g_jobstts_input_latched_button) {
        current_input &= g_jobstts_input_latched_button;
    } else if (current_input == 0) {
        g_jobstts_input_latched_button = 0;
        g_jobstts_input_activation_timer += speed;
    } else {
        if (g_jobstts_input_activation_timer < 4) {
            if (current_input & (PSX_PAD_UP | PSX_PAD_RIGHT | PSX_PAD_DOWN | PSX_PAD_LEFT)) {
                current_input &= PSX_PAD_UP | PSX_PAD_RIGHT | PSX_PAD_DOWN | PSX_PAD_LEFT;
                g_jobstts_input_activation_timer = 10;
            }
        }
        if (g_jobstts_input_activation_timer >= 4) {
            g_jobstts_input_activation_timer = 0;
            for (index = 0; index < 16; index++) {
                bit = 1 << index;
                if (current_input & bit) {
                    current_input &= bit;
                    g_jobstts_input_latched_button = bit;
                }
            }
        } else {
            current_input = 0;
            g_jobstts_input_latched_button = 0;
        }
    }
    filtered_input = current_input;
    if (g_jobstts_input_activation_timer >= 10) {
        g_jobstts_input_activation_timer = 10;
    }
    g_jobstts_input_newly_pressed = filtered_input;
    for (index = 0; index < 16; index++) {
        s32 mask = 1 << index;

        if (mask & g_jobstts_input_newly_pressed) {
            if (mask & g_jobstts_input_previous) {
                g_jobstts_input_newly_pressed &= ~mask;
            } else {
                g_jobstts_input_previous |= mask;
            }
        } else {
            g_jobstts_input_previous &= ~mask;
        }
    }
    counter_base = g_jobstts_input_repeat_counters;
    for (index = 0, counter = counter_base; index < 16; index++, counter++) {
        if (index == 4) {
            counter = counter_base + 12;
            index = 12;
        }
        if (current_input & (1 << index)) {
            (*counter)++;
        } else {
            *counter = 0;
        }
        if ((*counter >= initial_delay && *counter % repeat_period != 0)
            || (*counter >= 2 && *counter < initial_delay)) {
            current_input &= ~(1 << index);
        }
        if (*counter >= initial_delay) {
            current_input |= 1 << (index + 16);
        }
        if ((*counter >= initial_delay && *counter % secondary_repeat_period != 0)
            || (*counter >= 2 && *counter < initial_delay << 1)) {
            filtered_input &= ~(1 << index);
        }
    }
    current_input &= 0xf00ff00f;
    filtered_input &= 0xf00ff00f;
    current_input |= g_jobstts_input_newly_pressed & 0x0ff0;
    filtered_input |= g_jobstts_input_newly_pressed & 0x0ff0;
    g_jobstts_input_primary_repeat = current_input;
    g_jobstts_input_secondary_repeat = filtered_input;
}
