#include "fft/battle.h"
#include "psx/types.h"

/* Run a weather display command.
 *
 * Command 0x8b reads the weather from script variable 0x23 and re-issues the
 * set-up commands for values 0-1, 2, 3 and 4, choosing each variant by the
 * snow bit of the map weather flags. The weather local is an s8: compares on
 * the promoted value copy it into a scratch register for the 3 and 4 tests,
 * as the target does. */
void battle_map_run_weather_command(s32 command) {
    s8 weather;

    switch (command & 0xff) {
    case MAP_WEATHER_COMMAND_RESTORE_LIGHTING:
        battle_map_light_state_command(MAP_LIGHT_COMMAND_RESTORE_BACKGROUND_GRADIENT, 0);
        SetColorMatrix(g_battle_map_lightning_state.color_matrices);
        SetBackColor(g_battle_map_back_color_red, g_battle_map_back_color_green, g_battle_map_back_color_blue);
        break;
    case MAP_WEATHER_COMMAND_APPLY_SCRIPT_WEATHER:
        weather = battle_script_get_variable(EVENT_SCRIPT_VAR_WEATHER);
        g_battle_map_lightning_mode = MAP_LIGHTNING_OFF;
        weather &= 7;
        if (weather < BATTLE_WEATHER_RAIN) {
            battle_sound_stop_weather_sfx();
            battle_map_run_weather_command(MAP_WEATHER_COMMAND_CLEAR);
            battle_map_run_weather_command(MAP_WEATHER_COMMAND_RESTORE_LIGHTING);
        }
        if (weather == BATTLE_WEATHER_RAIN) {
            if (g_battle_map_weather_flags & BATTLE_MAP_WEATHER_FLAG_SNOW) {
                battle_sound_stop_weather_sfx();
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_LIGHT_SNOW);
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_RESTORE_LIGHTING);
            } else {
                battle_sound_stop_weather_sfx();
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_LIGHT_RAIN);
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_RESTORE_LIGHTING);
            }
        }
        if (weather == BATTLE_WEATHER_STORM) {
            if (g_battle_map_weather_flags & BATTLE_MAP_WEATHER_FLAG_SNOW) {
                battle_sound_stop_weather_sfx();
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_SNOW);
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_RESTORE_LIGHTING);
            } else {
                g_battle_sound_primary_weather_sfx_id = MAIN_SOUND_ID(MAIN_SOUND_RESOURCE_ENV, MAIN_ENV_SFX_RAIN);
                if (g_battle_sound_weather_sfx_enabled != 0) {
                    main_sound_play_bg_sfx(MAIN_SOUND_ID(MAIN_SOUND_RESOURCE_ENV, MAIN_ENV_SFX_RAIN));
                }
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_RAIN);
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_RESTORE_LIGHTING);
            }
        }
        if (weather == BATTLE_WEATHER_STRONG_STORM) {
            if (g_battle_map_weather_flags & BATTLE_MAP_WEATHER_FLAG_SNOW) {
                battle_sound_stop_weather_sfx();
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_HEAVY_SNOW);
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_RESTORE_LIGHTING);
            } else {
                battle_sound_stop_weather_sfx();
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_HEAVY_RAIN);
                battle_map_run_weather_command(MAP_WEATHER_COMMAND_LIGHTNING);
                g_battle_sound_primary_weather_sfx_id = MAIN_SOUND_ID(MAIN_SOUND_RESOURCE_ENV, MAIN_ENV_SFX_RAIN);
                if (g_battle_sound_weather_sfx_enabled != 0) {
                    main_sound_play_bg_sfx(MAIN_SOUND_ID(MAIN_SOUND_RESOURCE_ENV, MAIN_ENV_SFX_RAIN));
                }
            }
        }
        break;
    case MAP_WEATHER_COMMAND_THUNDER:
        g_battle_map_lightning_mode = MAP_LIGHTNING_ONCE;
        g_battle_map_lightning_state.timer = 0x3c;
        g_battle_sound_primary_weather_sfx_id = MAIN_SOUND_ID(MAIN_SOUND_RESOURCE_ENV, MAIN_ENV_SFX_RAIN);
        if (g_battle_sound_weather_sfx_enabled != 0) {
            main_sound_play_bg_sfx(MAIN_SOUND_ID(MAIN_SOUND_RESOURCE_ENV, MAIN_ENV_SFX_RAIN));
        }
        g_battle_map_lightning_state.color_matrices[1].m[0][0] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[0][1] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[0][2] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[1][0] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[1][1] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[1][2] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[2][0] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[2][1] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[2][2] = 0;
        battle_map_run_weather_command(MAP_WEATHER_COMMAND_RESTORE_LIGHTING);
        break;
    case MAP_WEATHER_COMMAND_LIGHTNING:
        g_battle_map_lightning_mode = MAP_LIGHTNING_RANDOM;
        g_battle_map_lightning_state.timer = 0x3c;
        g_battle_map_lightning_state.color_matrices[1].m[0][0] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[0][1] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[0][2] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[1][0] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[1][1] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[1][2] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[2][0] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[2][1] = 0;
        g_battle_map_lightning_state.color_matrices[1].m[2][2] = 0;
        battle_map_run_weather_command(MAP_WEATHER_COMMAND_RESTORE_LIGHTING);
        break;
    case MAP_WEATHER_COMMAND_SNOW:
        g_battle_map_weather_fall_speed = 1;
        g_battle_map_weather_primary_speed = 4;
        battle_map_init_weather_particles(MAP_WEATHER_COMMAND_INIT_SNOW_PARTICLES);
        break;
    case MAP_WEATHER_COMMAND_LIGHT_SNOW:
        g_battle_map_weather_fall_speed = 1;
        g_battle_map_weather_primary_speed = 1;
        battle_map_init_weather_particles(MAP_WEATHER_COMMAND_INIT_SNOW_PARTICLES);
        break;
    case MAP_WEATHER_COMMAND_HEAVY_SNOW:
        g_battle_map_weather_fall_speed = 3;
        g_battle_map_weather_primary_speed = 0x10;
        battle_map_init_weather_particles(MAP_WEATHER_COMMAND_INIT_SNOW_PARTICLES);
        break;
    case MAP_WEATHER_COMMAND_RAIN:
        g_battle_map_weather_fall_speed = 5;
        g_battle_map_weather_primary_speed = 6;
        g_battle_map_rain_secondary_fall_speed = 9;
        battle_map_init_weather_particles(MAP_WEATHER_COMMAND_INIT_RAIN_PARTICLES);
        break;
    case MAP_WEATHER_COMMAND_LIGHT_RAIN:
        g_battle_map_weather_fall_speed = 3;
        g_battle_map_weather_primary_speed = 5;
        g_battle_map_rain_secondary_fall_speed = 8;
        battle_map_init_weather_particles(MAP_WEATHER_COMMAND_INIT_RAIN_PARTICLES);
        break;
    case MAP_WEATHER_COMMAND_HEAVY_RAIN:
        g_battle_map_weather_fall_speed = 5;
        g_battle_map_weather_primary_speed = 9;
        g_battle_map_rain_secondary_fall_speed = 0x12;
        battle_map_init_weather_particles(MAP_WEATHER_COMMAND_INIT_RAIN_PARTICLES);
        break;
    case MAP_WEATHER_COMMAND_CLEAR:
        g_battle_map_weather_effect_mode = MAP_WEATHER_COMMAND_CLEAR;
        g_battle_map_lightning_mode = MAP_LIGHTNING_OFF;
        break;
    }
}
