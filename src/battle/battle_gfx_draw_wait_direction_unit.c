#include "fft/battle_camera.h"
#include "fft/battle_gfx.h"
#include "psx/types.h"

/*
 * Queue the acting unit's wait-direction indicator into the ordering table at
 * the depth of a point 48 world units above its feet.
 */
void battle_gfx_draw_wait_direction_unit(void) {
    SVECTOR base;
    SVECTOR raised;
    VECTOR transformed;
    long flag;
    battle_unit_t* unit;
    s32 depth;
    u32* ot;

    SetRotMatrix(&g_battle_camera_matrix);
    SetTransMatrix(&g_battle_camera_matrix);
    unit = battle_unit_get_source();
    main_util_set_svector(&base, unit->world_coords.vx, unit->world_coords.vy, unit->world_coords.vz);
    RotTrans(&base, &transformed, &flag);
    battle_camera_get_tile_sort_point_unbiased(&unit->world_coords, &raised);
    raised.vy -= 48;
    RotTrans(&raised, &transformed, &flag);
    depth = transformed.vz / 4;
    ot = main_gfx_get_otag();
    battle_gfx_draw_wait_direction_arrows(&base, &ot[depth]);
}
