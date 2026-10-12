#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * Read event script variable `variable_id` the way a script does, through
 * the Zero and Add variable commands, so that bit-packed and special
 * variables resolve identically. The comparison-32-bit result those commands
 * compute into is saved and restored around the read.
 *
 * Reading the combined weather/time variable first rebuilds it: time of day
 * in bit 15 and weather in bits 12..14.
 */
s32 script_get_variable(s32 variable_id) {
    s32 saved = *g_world_script_variables;
    s32 result;

    if (variable_id == EVENT_SCRIPT_VAR_WEATHER_AND_TIME) {
        s32 tod;
        s32 wth;

        tod = script_get_variable(EVENT_SCRIPT_VAR_TIME_OF_DAY);
        tod &= 1;
        wth = script_get_variable(EVENT_SCRIPT_VAR_WEATHER);
        wth &= 7;
        tod <<= 15;
        wth <<= 12;
        world_script_set_variable(EVENT_SCRIPT_VAR_WEATHER_AND_TIME, tod | wth);
    }
    world_script_run_variable_command(EVENT_OPCODE_ZERO_VARIABLE, EVENT_SCRIPT_VAR_COMPARISON_RESULT, 0, 0);
    world_script_run_variable_command(EVENT_OPCODE_ADD_VARIABLE, EVENT_SCRIPT_VAR_COMPARISON_RESULT, variable_id, 0);
    result = *g_world_script_variables;
    *g_world_script_variables = saved;
    return result;
}
