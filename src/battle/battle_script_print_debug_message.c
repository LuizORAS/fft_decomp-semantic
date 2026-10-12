#include "fft/battle_event.h"
#include "fft/battle_thread.h"

/* DebugPause: print "PAUSE" to the debug font stream, yield one frame and set
 * g_battle_script_debug_paused, which no code reads. The script continues. */
void battle_script_print_debug_message(void) {
    FntPrint(g_battle_script_pause_message);
    battle_thread_yield();
    g_battle_script_debug_paused = 1;
}
