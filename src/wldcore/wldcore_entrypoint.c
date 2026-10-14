#include "fft/wldcore.h"
#include "psx/types.h"

/* WLDCORE's entry point, run by MAIN's world loader (main_overlay_exec_wldcore_and_world_bin): one
 * world map visit per pass of the outer loop, until the visit hands over to something else.
 *
 * A pass first runs the formation an event asked for (wldcore_system_run_requested_formation, then returns 0),
 * then sets the visit up (wldcore_system_init_subsystems), fades in and runs the frame loop while
 * MAIN_SYSTEM_FLAG_RUNNING is set: the disc read, the audio queue, the fade, the dot pulse, the menu
 * level's step, the backdrop capture, then a frame with the live map (MAIN_SYSTEM_FLAG_DRAW_MAP) or
 * over the captured backdrop. MAIN_SYSTEM_FLAG_RESTART starts another visit; otherwise the visit
 * ends (wldcore_system_finalize_loop) and the world result is returned: 5 back to the title, 4 a Deep
 * Dungeon battle, 2 a tutorial event or a replayed story event, 1 a random battle, 0 a story event. */
s32 wldcore_entrypoint(void) {
    s32 buffer_index;

    for (;;) {
        if (wldcore_system_run_requested_formation() != 0) {
            return 0;
        }
        wldcore_system_init_subsystems();
        wldcore_gfx_start_screen_fade(0, 0x10);
        for (;;) {
            if ((g_main_system_flags & MAIN_SYSTEM_FLAG_RUNNING) == 0) {
                break;
            }
            main_file_poll_load(&g_main_file_cd_state);
            wldcore_sound_process_audio_queue();
            buffer_index = GsGetActiveBuff();
            g_active_graphics_buffer_index = buffer_index;
            GsSetWorkBase(&g_wldcore_gfx_world_primitive_buffers + buffer_index * 0xE000);
            wldcore_gfx_step_screen_fade();
            wldcore_map_pulse_dot_colors();
            wldcore_menu_dispatch_idle_handler();
            wldcore_gfx_update_map_backdrop();
            if (g_main_system_flags & MAIN_SYSTEM_FLAG_DRAW_MAP) {
                wldcore_gfx_draw_world_frame_with_map();
            } else {
                wldcore_gfx_draw_world_frame();
            }
            if (g_main_system_flags & MAIN_SYSTEM_FLAG_DISPLAY_OFF) {
                SetDispMask(0);
            } else {
                SetDispMask(1);
            }
            main_system_frame_hook();
        }
        if (g_main_system_flags & MAIN_SYSTEM_FLAG_RESTART) {
            continue;
        }
        wldcore_system_finalize_loop();
        {
            s32 flags = g_main_system_flags;

            if (flags & MAIN_SYSTEM_FLAG_RETURN_TO_TITLE) {
                return 5;
            }
            if (flags & MAIN_SYSTEM_FLAG_DEEP_DUNGEON_BATTLE) {
                return 4;
            }
            if (flags & MAIN_SYSTEM_FLAG_TUTORIAL_EVENT) {
                return 2;
            }
            /* Returning through display_mask keeps two separate `return 2`
             * blocks; a plain `return 2` here cross-jumps into the one above. */
            {
                s32 display_mask;

                if (flags & MAIN_SYSTEM_FLAG_STORY_EVENT_REPLAY) {
                    display_mask = 2;
                } else {
                    display_mask = (flags & MAIN_SYSTEM_FLAG_RANDOM_BATTLE) > 0;
                }
                return display_mask;
            }
        }
    }
}
