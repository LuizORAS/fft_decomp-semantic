#include "fft/main.h"
#include "psx/types.h"

/* Drop the saved copy of a music record: clear its SUZUKI_MUSIC_STATUS_SNAPSHOT flag and free the
 * snapshot (main_smd_free_snapshots). It is mode 0 of main_smd_dispatch_snapshot, which nothing calls. */
void main_smd_discard_snapshot(suzuki_music_t* music) {
    u16 status;

    status = music->status;
    if ((status & SUZUKI_MUSIC_STATUS_SNAPSHOT) != 0) {
        music->status = status & ~SUZUKI_MUSIC_STATUS_SNAPSHOT;
        main_smd_free_snapshots(music);
    }
}
