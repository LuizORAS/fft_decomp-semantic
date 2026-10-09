#include "fft/event_attack.h"
#include "psx/types.h"

void attack_gfx_init_frame_ft4_array(void) {
    s32 i;

    for (i = 0; i < 10; i += 2) {
        battle_gfx_init_default_poly_ft4(&g_attack_gfx_frame_ft4[i]);
        battle_gfx_init_default_poly_ft4(&g_attack_gfx_frame_ft4[i + 1]);
        g_attack_gfx_frame_ft4[i].tpage = GetTPage(0, 0, 0x140, 0);
        g_attack_gfx_frame_ft4[i + 1].tpage = GetTPage(0, 2, 0x3c0, 0x100);
        SetSemiTrans(&g_attack_gfx_frame_ft4[i + 1], 1);
    }
}
