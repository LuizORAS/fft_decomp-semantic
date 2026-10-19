#include "fft/open.h"
#include "psx/types.h"

/* Stop the movie: wait for the GPU, remove the MDEC output callback and the CD ring buffer, pause the CD
 * audio when `pause_cd_audio` is set (open_movie_pause_cd_audio), and clear
 * OPEN_SYSTEM_RUNTIME_FLAG_MOVIE_STREAMING. The movie controllers call it on Start, and the stream step at
 * end_frame (pausing the audio) or video_end_frame (leaving it playing). */
void open_movie_stop_stream(s32 pause_cd_audio) {
    DrawSync(0);
    DecDCToutCallback(0);
    StUnSetRing();

    if (pause_cd_audio != 0) {
        open_movie_pause_cd_audio();
    }

    g_open_system_runtime_flags &= ~OPEN_SYSTEM_RUNTIME_FLAG_MOVIE_STREAMING;
}
