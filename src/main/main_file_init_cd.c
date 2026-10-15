#include "fft/main.h"
#include "psx/types.h"

/* Set the RAM size to 2 MB (SetMem), initialize libcd with its debug output off, then stop
 * any read and pause the drive. Boot and each per-session reset call it. */
void main_file_init_cd(void) {
    SetMem(2);
    CdInit();
    CdSetDebug(0);
    main_file_cancel_and_pause_cd(&g_main_file_cd_state);
    g_main_file_still_loading = 0;
}
