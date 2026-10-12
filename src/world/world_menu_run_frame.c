#include "fft/world.h"
#include "psx/pad.h"
#include "psx/types.h"

/* The six input words at 0x801aed44 are g_world_menu_frame_input_state,
 * but a struct global makes GCC keep a base register for the field accesses
 * (see world_input_update_event_state), so they are separate scalars here. */
extern u32 g_world_menu_frame_current_input, g_world_menu_frame_previous_input,
    g_world_menu_frame_initial_repeat_counter, g_world_menu_frame_repeat_counter,
    g_world_menu_frame_secondary_repeat_counter;

/* Run one frame of WORLD's menus inside another module's frame (WLDCORE's world map, OPEN's
 * title menus) and return the menu result, g_world_menu_pending_selection[0] (-2 while pending; 0 also
 * asks WLDCORE to travel to the chosen map). It derives the new and repeated presses with the input
 * timing of the game options, masked by g_world_menu_allowed_buttons and dropped while a screen request
 * runs; steps the formation, Brave Story, card, tutorial, debug list and fixed number panel requests;
 * hands the input to the menu threads and yields to them once, starting a five-frame cooldown
 * (g_world_event_input_suppression_frames) when a thread started or stopped. It sets g_world_menu_hosted_frame.
 * Sibling of world_input_update_event_state, which WORLD's own screens use. */
s16 world_menu_run_frame(u32* otag, u32 input) {
    s32 profile;
    u32 previous_input;
    u32 filtered_input;
    s32 initial_delay;
    s32 repeat_delay;
    s32 secondary_delay;
    u32 counter;
    s32 thread_id;
    s32 status;

    profile = g_main_game_options.fields.finger_cursor_repeat_speed;
    g_main_input_repeat_initial_delay = g_world_input_timing_profiles[profile].initial_delay;
    g_main_input_repeat_period = g_world_input_timing_profiles[profile].repeat_period;
    g_main_input_secondary_repeat_period = g_world_input_timing_profiles[profile].secondary_repeat_period;
    g_main_menu_scroll_accel_delay = g_world_input_timing_profiles[profile].scroll_accel_delay;
    g_main_menu_scroll_slow_step = g_world_input_timing_profiles[profile].scroll_slow_step;
    g_main_menu_scroll_fast_step = g_world_input_timing_profiles[profile].scroll_fast_step;
    if (input == 0) {
        g_world_menu_frame_initial_repeat_counter = 0;
        g_world_menu_frame_repeat_counter = 0;
        g_world_menu_frame_secondary_repeat_counter = 0;
    }

    previous_input = g_world_menu_frame_current_input;
    g_world_menu_frame_current_input = input;
    g_world_menu_frame_new_buttons = 0;
    g_world_menu_frame_previous_input = previous_input;

    if (previous_input != input || (filtered_input = input & (PSX_PAD_L1 | PSX_PAD_R1 | PSX_PAD_DPAD_MASK)) == 0) {
        g_world_menu_frame_new_buttons = ~previous_input & input;
        g_world_menu_frame_initial_repeat_counter = 0;
    } else {
        initial_delay = g_main_input_repeat_initial_delay / g_world_event_speed;
        counter = ++g_world_menu_frame_initial_repeat_counter;
        if (counter == initial_delay) {
            g_world_menu_frame_new_buttons = filtered_input;
        }
        if (counter >= initial_delay) {
            repeat_delay = g_main_input_repeat_period / g_world_event_speed;
            counter = ++g_world_menu_frame_repeat_counter;
            g_world_menu_frame_secondary_repeat_counter++;
            if (counter >= repeat_delay) {
                g_world_menu_frame_repeat_counter = 0;
                if ((input & PSX_PAD_SQUARE) != 0) {
                    g_world_menu_frame_new_buttons = input & (PSX_PAD_RIGHT | PSX_PAD_LEFT);
                } else {
                    g_world_menu_frame_new_buttons = input & PSX_PAD_DPAD_MASK;
                }
            }
            secondary_delay = g_main_input_secondary_repeat_period / g_world_event_speed;
            if (g_world_menu_frame_secondary_repeat_counter >= secondary_delay) {
                g_world_menu_frame_secondary_repeat_counter = 0;
                if ((g_world_menu_frame_current_input & PSX_PAD_SQUARE) != 0) {
                    g_world_menu_frame_new_buttons
                        |= g_world_menu_frame_current_input & (PSX_PAD_L1 | PSX_PAD_R1 | PSX_PAD_UP | PSX_PAD_DOWN);
                } else {
                    g_world_menu_frame_new_buttons |= g_world_menu_frame_current_input & (PSX_PAD_L1 | PSX_PAD_R1);
                }
            }
        } else {
            g_world_menu_frame_repeat_counter = 0;
            g_world_menu_frame_secondary_repeat_counter = 0;
        }
    }

    g_world_menu_frame_new_buttons &= g_world_menu_allowed_buttons;
    if (g_world_formation_open_request_state != WORLD_SCREEN_REQUEST_IDLE) {
        g_world_menu_frame_new_buttons = 0;
    }
    if (g_world_tutorial_categories_request_state != WORLD_SCREEN_REQUEST_IDLE) {
        g_world_menu_frame_new_buttons = 0;
    }
    if (g_world_debug_variable_list_request_state != 0) {
        g_world_menu_frame_new_buttons = 0;
    }
    if (g_world_card_menu_request_state != WORLD_SCREEN_REQUEST_IDLE) {
        g_world_menu_frame_new_buttons = 0;
    }
    if (g_world_brave_story_request_state != WORLD_SCREEN_REQUEST_IDLE) {
        g_world_menu_frame_new_buttons = 0;
    }
    if (g_world_fixed_number_panel_request_state != 0) {
        g_world_menu_frame_new_buttons = 0;
    }
    g_world_menu_hosted_frame = 1;
    world_formation_update_open_request();
    world_menu_update_brave_story_request();
    world_card_update_menu_request();
    world_menu_update_tutorial_categories_request();
    world_menu_update_debug_variable_list_request();
    world_menu_update_fixed_number_panel_request();
    g_world_menu_pending_selection[0] = -2;
    g_world_gfx_current_otag_entry = otag;
    g_world_sound_pending_sfx_id = -1;
    world_menu_draw_active_window_frames();
    world_script_load_event_input_state((const world_menu_input_state_t*)&g_world_menu_frame_new_buttons);
    if (g_world_menu_frame_new_buttons != 0x100 && g_world_menu_frame_new_buttons != 0) {
        g_world_event_input_suppression_frames = 5;
    }

    thread_id = 1;
    do {
        g_world_menu_thread_status_snapshot[thread_id] = world_thread_is_running_asm(thread_id);
        thread_id++;
    } while (thread_id < 16);

    world_thread_yield();
    thread_id = 1;
    do {
        status = g_world_menu_thread_status_snapshot[thread_id];
        if (status != world_thread_is_running_asm(thread_id)) {
            g_world_event_input_suppression_frames = 5;
        }
        thread_id++;
    } while (thread_id < 16);
    if (g_world_event_input_suppression_frames != 0) {
        g_world_event_input_suppression_frames--;
    }
    if (g_world_menu_companion_executable_request_index != 0) {
        g_world_menu_companion_executable_request_index = 0;
    }
    g_world_frame_counter++;
    g_world_menu_packet_buffer_index = (g_world_menu_packet_buffer_index + 1) & 1;
    if (g_world_sound_pending_sfx_id != -1) {
        main_sound_play_sfx_find_channel(g_world_sound_pending_sfx_id);
    }
    if (g_world_menu_pending_selection[0] == 0) {
        world_script_set_variable(EVENT_SCRIPT_VAR_CURRENT_MAP, g_world_menu_pending_selection[1]);
        g_main_system_flags |= MAIN_SYSTEM_FLAG_REQUEST_DESTINATION;
    }
    if (g_world_menu_input_block_frames != 0) {
        g_world_menu_input_block_frames--;
    }
    return g_world_menu_pending_selection[0];
}
