#include "fft/battle_map.h"
#include "fft/battle_script.h"
#include "fft/event_attack.h"
#include "fft/script.h"
#include "psx/types.h"

/* Run one step of the map load that sets up a battle (battle_state_run_battle_setup, under Now Loading
 * or the start banner) and return the next step, 0xd when it is done. A step that waits returns itself:
 * - 0: reset the camera and the map palettes, roll the weather from the calendar for a random battle
 *   (world result 1) and load the weather particle palette;
 * - 2-3: start and finish the map load (battle_map_load_data);
 * - 4-6: open the ENTD and load its units, then the unit sprite shapes and sequences unless they are
 *   already loaded;
 * - 7: load the queued unit graphics, unless the battle uses the world map's debug battle style;
 * - 8: wait for the deployment to end (g_battle_deployment_finished), then set up the deployed units'
 *   graphics;
 * - 9: load them;
 * - 10: wait until the deployment screen stops running, then resync the scenario music and set the map
 *   darkness ATTACK.OUT carries (attack_sound_resync_scenario_music_and_apply_map_darkness);
 * - 11: after a random battle, play a random battle track;
 * - 12: start the weather sounds, seed the three colour fades from the loaded map, upload the CLUT rows,
 *   reset the turn clock and set up the AI workspace.
 * battle_map_step_init_sequence is the same load for a map entered later in the battle. */
s32 battle_map_step_setup_sequence(s32 map_id, s32 step, s32 deployment_running) {
    s32 result;
    u8 unused_18[4];

    switch (step) {
    case 0:
        /* Called with no argument: the callee receives map_id, still in $a0
         * (passing it explicitly changes the register allocation). */
        ((void (*)(void))main_sound_stop_sfx)();
        battle_camera_init_defaults();
        battle_map_init_palette_state();
        if (g_main_system_frontend_world_result == 1) {
            battle_map_randomize_weather_from_calendar();
        }
        step++;
        battle_map_load_palette_data(&g_battle_map_weather_particle_palette, MAP_CLUT_ROW_SPRITES, 0, 0);
        g_main_gfx_unit_graphics_load_queue.count = 0;
        break;
    case 1:
        step++;
        break;
    case 2:
        if (battle_map_load_data(map_id, MAP_LOAD_COMMAND_START) != 0) {
            step++;
        }
        break;
    case 3:
        if (battle_map_load_data(map_id, MAP_LOAD_COMMAND_POLL) == 0) {
            step++;
        }
        break;
    case 4:
        if (battle_unit_select_and_open_entd() != 0) {
            step++;
        }
        break;
    case 5:
        if (battle_unit_poll_and_load_entd_units() != 0) {
            break;
        }
        if (g_battle_unit_graphics_load_complete != 0) {
            battle_gfx_rewind_unit_graphics_loader(map_id);
            step = 7;
        } else {
            step++;
            battle_gfx_reset_unit_graphics_load_state();
        }
        break;
    case 6:
        if (battle_gfx_step_load_unit_shp_seq_data() == 8) {
            g_battle_unit_graphics_load_complete = 1;
            battle_gfx_rewind_unit_graphics_loader(map_id);
            step++;
        }
        break;
    case 7:
        if (script_get_variable(EVENT_SCRIPT_VAR_WORLD_DEBUG_BATTLE_STYLE) != 0) {
            step++;
            break;
        }
        do {
            result = battle_gfx_step_queued_unit_graphics_load();
        } while (result == 2);
        if (result == 0) {
            step++;
        }
        break;
    case 8:
        if (g_battle_deployment_finished != 0) {
            step++;
            battle_gfx_init_deployment_and_reset_unit_graphics_state();
            g_battle_deployment_skipped = 0;
        }
        break;
    case 9:
        do {
            result = battle_gfx_step_queued_unit_graphics_load();
        } while (result == 2);
        if (result == 0) {
            step++;
        }
        break;
    case 10:
        if (deployment_running == 0) {
            step++;
            attack_sound_resync_scenario_music_and_apply_map_darkness();
        }
        break;
    case 11:
        if (g_main_system_frontend_world_result == 1
            || script_get_variable(EVENT_SCRIPT_VAR_WORLD_DEBUG_BATTLE_STYLE) != 0) {
            main_sound_unload_current_scenario_music();
            /* Called with no argument: the sound id is whatever
             * main_sound_unload_current_scenario_music left in $a0. */
            ((void (*)(void))main_sound_stop_sfx)();
            main_sound_set_current_music_track(g_battle_random_battle_music_tracks[rand() / 0x2000]);
        }
        step++;
        break;
    case 12:
        battle_sound_start_weather_sfx();
        /* Seed the three colour fades from the colours the loaded map set. */
        battle_map_init_background_gradient();
        battle_map_init_ambient_light();
        battle_map_init_darkness();
        battle_map_upload_clut_rows();
        battle_turn_run_clock(BATTLE_BETWEEN_TURN_MODE_RESET);
        battle_ai_init_workspace();
        step++;
        break;
    }
    return step;
}
