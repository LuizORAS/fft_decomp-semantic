#include "fft/main.h"
#include "psx/libcd.h"

/* Mark descriptor idle, flush libcd (CdFlush) and send Pause until the drive accepts it,
 * then wait 3 vertical blanks. */
void main_file_cancel_and_pause_cd(main_file_load_descriptor_t* descriptor) {
    descriptor->state = MAIN_FILE_LOAD_STATE_IDLE;
    CdFlush();
    while (CdControlB(CdlPause, 0, 0) == 0) { }
    VSync(3);
}
