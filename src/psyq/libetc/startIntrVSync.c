#include "psx/libetc.h"

/* Start the VBlank dispatch: set root counter 1 to count horizontal blanks (mode 0x107, which VSync
 * measures with), reset the VBlank count, clear the eight callbacks, install trapIntrVSync on interrupt 0 and
 * return setIntrVSync, which startIntr puts in the dispatch table. */
psyq_callback_setter_t startIntrVSync(void) {
    *g_psyq_etc_timer1_mode = PSYQ_ETC_VBLANK_COUNTER_MODE;
    g_psyq_etc_vblank_count = 0;
    psyq_etc_clear_vblank_callback_words((u32*)g_psyq_etc_vblank_callbacks, PSYQ_ETC_CALLBACK_SLOTS);
    InterruptCallback(PSYQ_IRQ_VBLANK, trapIntrVSync);
    return setIntrVSync;
}
