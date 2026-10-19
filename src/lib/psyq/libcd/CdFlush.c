#include "psx/libcd.h"

/* Acknowledge the drive's pending interrupts and reset the library's command state (CD_flush).
 * main_file_cancel_and_pause_cd calls it, and cd_read_retry before every read. */
void CdFlush(void) {
    CD_flush();
}
