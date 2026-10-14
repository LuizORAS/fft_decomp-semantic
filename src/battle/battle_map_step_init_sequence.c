#include "fft/battle.h"
#include "psx/types.h"

/* Run one step of the map load of the MAP_INITIALIZE states (battle_state_handle_map_init_state,
 * battle_state_handle_change_map_init_state), which load a new map behind a black screen during the
 * battle, and return the next step, 0xd when it is done. A step that waits returns itself. It follows
 * battle_map_step_setup_sequence without the deployment: step 0 also requests SMALL.OUT and step 1 waits
 * for it, the unit sprite shapes and sequences are not reloaded, and step 8 places the units at the
 * coordinates the deployment left (g_main_deployed_unit_map_coordinates). `unused_18` reproduces the
 * target's unreferenced frame slot at sp+0x18.
 *
 * Jump table: .rodata 0x80068260, 13 words. */
s32 battle_map_step_init_sequence(s32 map_id, s32 step) {
    s32 result;
    u8 unused_18[4];

    switch (step) {
    case 0:
        /* No sound id is loaded: the callee receives map_id, still in $a0. */
        main_sound_stop_sfx(map_id);
        step++;
        battle_camera_init_defaults();
        battle_map_init_palette_state();
        battle_map_load_palette_data(g_battle_map_weather_particle_palette, MAP_CLUT_ROW_SPRITES, 0, 0);
        g_main_gfx_unit_graphics_load_queue.count = 0;
        battle_menu_request_open_small();
        break;
    case 1:
        if (battle_menu_poll_small_load() == 0) {
            step++;
        }
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
        if (battle_unit_poll_and_load_entd_units() == 0) {
            step++;
        }
        break;
    case 6:
        battle_gfx_clear_four_state_words(map_id);
        step++;
        break;
    case 7:
        do {
            result = battle_gfx_step_queued_unit_graphics_load();
        } while (result == 2);
        if (result == 0) {
            step++;
        }
        break;
    case 8:
        battle_unit_build_deployed_units_data((battle_deployed_coords_t*)g_main_deployed_unit_map_coordinates, 0);
        step++;
        break;
    case 9:
    case 10:
        step++;
        break;
    case 11:
        if (g_main_system_frontend_world_result == 1
            || battle_script_get_variable(EVENT_SCRIPT_VAR_WORLD_DEBUG_BATTLE_STYLE) != 0) {
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
