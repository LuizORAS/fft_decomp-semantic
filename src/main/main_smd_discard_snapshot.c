#include "fft/main.h"
#include "psx/types.h"

void main_smd_discard_snapshot(suzuki_music_t* music) {
    u16 status;

    status = music->status;
    if ((status & SUZUKI_MUSIC_STATUS_SNAPSHOT) != 0) {
        music->status = status & ~SUZUKI_MUSIC_STATUS_SNAPSHOT;
        main_smd_free_snapshots(music);
    }
}
