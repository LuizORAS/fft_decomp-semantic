#include "fft/world.h"
#include "psx/types.h"

/* One frame of the job wheel closing: over 20 frames the wheel turns a
 * quarter turn while its radius grows from 1.0 by 0x140 a frame. It ends by
 * setting the menu value WORLD_FORMATION_MENU_JOB_WHEEL_OPENING. */
void world_formation_step_job_wheel_close_animation(void) {
    s32 pos;

    if (g_world_job_wheel_closing == 0) {
        world_formation_start_job_wheel_close();
    }
    pos = g_world_job_wheel_close_frame * 0x140 + ONE;
    world_formation_draw_job_wheel_sprites(((g_world_job_wheel_close_frame + 1) << 10) / 20, pos, pos);
    g_world_job_wheel_close_frame++;
    if (g_world_job_wheel_close_frame >= 0x14) {
        g_world_job_wheel_closing = 0;
        g_world_formation_current_menu = WORLD_FORMATION_MENU_JOB_WHEEL_OPENING;
    }
}
