#include "fft/battle.h"
#include "fft/main.h"
#include "fft/script.h"

/* The top-level loop of the game: opening, then world map and battles until a
 * reset or the ending. It never returns; main_system_reset_game re-enters it from
 * the top with the stack restored.
 *
 * Each pass empties the inventory to the new-game stock and runs OPEN.BIN (after
 * main_boot_reset_game_state, unless the world map asked for the title screen
 * with result 5, which also skips the opening movie). An OPEN.BIN result of 0
 * goes straight into BATTLE.BIN; otherwise WLDCORE and WORLD run first and leave
 * their result in g_main_system_frontend_world_result. World map and BATTLE.BIN
 * then alternate until g_main_system_game_flow_state asks for a reset or the
 * ending. The ending sets event 0x12a, plays OPEN.BIN's ending cutscene and runs
 * BATTLE.BIN once more. */
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
                g_main_system_frontend_world_result = main_overlay_open_world_and_wldcore(1);
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
            main_sound_unload_scenario_music_and_tunes();
            script_set_variable(EVENT_SCRIPT_VAR_NEXT_SCENARIO, 1);
            script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, EVENT_ID_MALAK_REVIVED);
            main_overlay_exec_open_bin_ending();
            main_overlay_exec_battle_bin();
            main_gfx_load_zodiac_frame();
            battle_state_run_game_loop();
            main_system_stop_display_and_audio_for_game_reset();
        }
    }
}
