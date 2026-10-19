/* SCUS_942.21 0x800201b4..0x800202a7. */
#include "psx/libcd.h"

/* Set the SPU's main volume to full when it is silent and its CD input volume to full, turn the SPU and
 * its CD audio on, and mix the CD's channels straight through (0x80 to the same side, none across). */
s32 CD_initvol(void) {
    CdlATV volume;
    if (g_psyq_cd_spu_registers->current_main_volume_left == 0
        && g_psyq_cd_spu_registers->current_main_volume_right == 0) {
        g_psyq_cd_spu_registers->main_volume_left = 0x3fff;
        g_psyq_cd_spu_registers->main_volume_right = 0x3fff;
    }
    g_psyq_cd_spu_registers->cd_volume_left = 0x3fff;
    g_psyq_cd_spu_registers->cd_volume_right = 0x3fff;
    g_psyq_cd_spu_registers->control = 0xc001;
    volume.val0 = volume.val2 = 0x80;
    volume.val1 = volume.val3 = 0;
    psyq_cd_apply_volume(&volume);
    return 0;
}
