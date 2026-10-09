#include "fft/open.h"

void open_script_update_timing_and_record_values(void) {
    s32 step;
    s32 work;
    s32 index;

    step = g_open_script_state.dispatch.timing_step;
    if (step == 0) {
        return;
    }

    if (step > 0) {
        work = step + g_open_script_timing_fraction;
        g_open_script_timing_fraction = work & 0xff;
        work >>= 8;
        g_open_script_external_counter += work;
    } else {
        work = g_open_script_timing_fraction - step;
        g_open_script_timing_fraction = work & 0xff;
        work >>= 8;
        g_open_script_external_counter += work;
        work = -work;
    }

    for (index = 0; index < 16; index++) {
        if (g_open_script_state.records[index].flags & 1) {
            g_open_script_state.records[index].y += work;
            if (g_open_script_state.records[index].y < 0x10 || g_open_script_state.records[index].y > 0x100) {
                g_open_script_state.records[index].flags ^= 1;
            }
        }
    }
}
