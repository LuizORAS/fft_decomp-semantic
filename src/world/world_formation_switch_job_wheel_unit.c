#include "fft/world.h"
#include "psx/types.h"

/* Move the job wheel to the newly selected unit (the L1/R1 cycle, and the end of the grid's slide
 * in): a closing wheel turns straight to opening, an opening one rebuilds its jobs for the unit and
 * backs its animation up by the input repeat period (to at most 16 frames left), and a settled wheel
 * starts closing to reopen. Then it lays out the unit's portrait. */
void world_formation_switch_job_wheel_unit(void) {
    s32 step;
    /* The target frame is 0x30 bytes; 0x18 of unreferenced locals. */
    s32 unused[6];

    if (g_world_job_wheel_closing != 0) {
        g_world_formation_current_menu = WORLD_FORMATION_MENU_JOB_WHEEL_OPENING;
        g_world_job_wheel_closing = 0;
        g_world_job_wheel_unit_index = g_world_formation_selected_unit_index;
    } else {
        if (g_world_job_wheel_opening != 0) {
            step = g_world_job_wheel_open_frame;
            g_world_job_wheel_unit_index = g_world_formation_selected_unit_index;
            world_formation_build_job_wheel_availability();
            g_world_job_wheel_open_frame
                = step + g_main_input_secondary_repeat_period / world_gfx_get_vsync_mode_or_one();
            if (g_world_job_wheel_open_frame >= 0x11) {
                g_world_job_wheel_open_frame = 0x10;
            }
            g_world_formation_current_menu = WORLD_FORMATION_MENU_JOB_WHEEL_OPENING;
        } else {
            g_world_formation_current_menu = WORLD_FORMATION_MENU_JOB_WHEEL_CLOSING;
        }
    }
    world_formation_layout_job_wheel_portrait();
}
