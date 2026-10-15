#include "psx/libetc.h"

/* Start reading the pads: record `mode`, set the button word to all released, start LIBETC's interrupts
 * (ResetCallback) and the BIOS's pad handler (PAD_init2), and keep the BIOS from acknowledging the VBlank
 * itself (ChangeClearPAD(0)). main_boot_run_startup calls it with 0. */
void PadInit(int mode) {
    g_psyq_etc_pad_mode = mode;
    g_psyq_etc_pad_buttons = -1;
    ResetCallback();
    PAD_init2(0x20000001, &g_psyq_etc_pad_buttons);
    ChangeClearPAD(0);
}
