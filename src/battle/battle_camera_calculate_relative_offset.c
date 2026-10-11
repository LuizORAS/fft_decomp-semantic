#include "fft/battle.h"
#include "psx/types.h"

/* Offset a position for the current camera yaw: subtract 0xc from y, and move
 * x and z by half a tile (0xe) and by offset, with signs set by the quadrant. */
void battle_camera_calculate_relative_offset(const SVECTOR* input, SVECTOR* output, s32 offset) {
    s32 x;
    s32 z;

    output->vy = (u16)input->vy - 0xc;
    switch (g_battle_camera_render_state.vy & 0xc00) {
    case 0:
        x = (u16)input->vx + 0xe;
        output->vx = x - offset;
        z = (u16)input->vz - 0xe;
        output->vz = z + offset;
        break;
    case 0x400:
        x = (u16)input->vx + 0xe;
        output->vx = x - offset;
        z = (u16)input->vz + 0xe;
        output->vz = z - offset;
        break;
    case 0x800:
        x = (u16)input->vx - 0xe;
        output->vx = x + offset;
        z = (u16)input->vz + 0xe;
        output->vz = z - offset;
        break;
    case 0xc00:
        x = (u16)input->vx - 0xe;
        output->vx = x + offset;
        z = (u16)input->vz - 0xe;
        output->vz = z + offset;
        break;
    }
}
