#include "fft/battle.h"
#include "fft/main.h"
#include "psx/types.h"

/* Top-level loop, never returns (main_system_reset_game re-enters it with the stack restored).
 * Each pass resets the inventory to the new-game stock and runs the OPEN.BIN title, after
 * main_boot_reset_game_state unless the world map returned 5, which also skips the opening
 * movie. A title result of 0 goes straight into BATTLE.BIN; otherwise the world map (WLDCORE and
 * WORLD) and BATTLE.BIN alternate until a reset (flow state 2) or the ending (flow state 3). A
 * world result of 5 returns to the title without resetting the game state. The ending plays
 * OPEN.BIN's ending and runs BATTLE.BIN once more. */
void main_system_run_game_loop(void) {
    int open_mode;

    g_main_system_frontend_world_result = 0;

    for (;;) {
        main_item_init_new_game_inventory();

        if (g_main_system_frontend_world_result == 5) {
            open_mode = 1;
        } else {
            main_boot_reset_game_state();
            open_mode = 0;
        }

        if (main_overlay_exec_open_bin_main_loop(open_mode) == 0) {
            g_main_system_go_straight_to_battle = 1;
            g_main_system_frontend_world_result = 0;
        } else {
            g_main_system_go_straight_to_battle = 0;
        }

        do {
            if (g_main_system_go_straight_to_battle == 0) {
                g_main_system_frontend_world_result = main_overlay_exec_wldcore_and_world_bin(1);
            }

            if (g_main_system_frontend_world_result == 5) {
                break;
            }

            main_overlay_exec_battle_bin();
            battle_state_run_game_loop();
            main_sound_stop_weather_sfx_music();
            g_main_system_go_straight_to_battle = 0;

            if (g_main_system_game_flow_state == MAIN_GAME_FLOW_RESET) {
                main_system_stop_display_and_audio_for_game_reset();
                break;
            }
        } while (g_main_system_game_flow_state != MAIN_GAME_FLOW_ENDING);

        if (g_main_system_game_flow_state == MAIN_GAME_FLOW_ENDING) {
            main_sound_stop_weather_sfx_music();
            main_sound_unload_scenario_music_and_stop_tune();
            battle_script_set_variable(EVENT_SCRIPT_VAR_NEXT_SCENARIO, 1);
            /* With NEXT_SCENARIO set, the ATTACK scenario loader advances this
             * event to Things Obtained or Those Who Squirm in Darkness. */
            battle_script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, EVENT_ID_SEARCHING_FOR_ALMA);
            main_overlay_exec_open_bin_ending();
            main_overlay_exec_battle_bin();
            main_gfx_load_zodiac_frame();
            battle_state_run_game_loop();
            main_system_stop_display_and_audio_for_game_reset();
        }
    }
}
