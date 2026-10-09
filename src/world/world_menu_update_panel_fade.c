#include "fft/world.h"

/*
 * Update the menu panel's fade and submit its center and edge tiles.
 *
 * Mode 1 raises intensity toward 80; mode 2 lowers it toward 0. The nine
 * tile pairs taper away from the center in increments of 8.
 */
void world_menu_update_panel_fade(void) {
    world_fade_tile_frame_t* frame;
    s32 i;
    s32 intensity;

    if (g_world_menu_panel_fade_mode != 0) {
        frame = &g_world_menu_panel_fade_frames[g_world_menu_packet_buffer_index];
        if (g_world_menu_panel_fade_mode == 1 && g_world_menu_panel_fade_intensity < 80) {
            g_world_menu_panel_fade_intensity += 8;
        }
        if (g_world_menu_panel_fade_mode == 2) {
            if (g_world_menu_panel_fade_intensity != 0) {
                g_world_menu_panel_fade_intensity -= 8;
            } else {
                g_world_menu_panel_fade_mode = 0;
            }
        }
        frame->center.r0 = ((world_fade_intensity_t*)&g_world_menu_panel_fade_intensity)->bytes[0];
        frame->center.g0 = ((world_fade_intensity_t*)&g_world_menu_panel_fade_intensity)->bytes[0];
        frame->center.b0 = ((world_fade_intensity_t*)&g_world_menu_panel_fade_intensity)->bytes[0];
        intensity = g_world_menu_panel_fade_intensity;
        for (i = 0; i < 9; i++) {
            if (intensity != 0) {
                intensity -= 8;
            }
            /* The pointer walk preserves the target's operand order for the biased index. */
            (frame->backdrop + (8 - i))->r0 = intensity;
            (frame->backdrop + (8 - i))->g0 = intensity;
            (frame->backdrop + (8 - i))->b0 = intensity;
            frame->bar[i].r0 = intensity;
            frame->bar[i].g0 = intensity;
            frame->bar[i].b0 = intensity;
            world_gfx_draw_or_append_gpu_primitive(&frame->backdrop[8 - i]);
            world_gfx_draw_or_append_gpu_primitive(&frame->bar[i]);
        }
        world_gfx_draw_or_append_gpu_primitive(
            &g_world_menu_panel_fade_frames[g_world_menu_packet_buffer_index].center);
        world_gfx_draw_or_append_gpu_primitive(
            &g_world_menu_panel_fade_frames[g_world_menu_packet_buffer_index].draw_mode);
    }
}
