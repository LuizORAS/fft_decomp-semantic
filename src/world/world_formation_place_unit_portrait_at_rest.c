#include "fft/world.h"
#include "psx/types.h"

/* Draw a formation unit's sprite at its resting position (0x8B, 0x9E). */
void world_formation_place_unit_portrait_at_rest(s16 unit_index) {
    point16_t pos;

    pos.x = 0x8B;
    pos.y = 0x9E;
    world_formation_draw_unit_sprite(unit_index, &pos, 0, 6);
}
