#include "fft/open.h"

typedef struct open_movie_frame_header {
    u8 _unused_00[8];
    u32 frame_number;
    u8 _unused_0c[4];
    u16 width;
    u16 height;
} open_movie_frame_header_t;

#define OPEN_MOVIE_ACQUIRE_MAX_ATTEMPTS 0x80

/* Wait (up to 0x80 vertical blanks) for the next movie frame, track the
 * stream position, adopt a changed frame size (clearing the screen), and size
 * the decode rectangles: 24-bit rows take three halfwords per two pixels.
 * Returns the frame data, or NULL on timeout. */
void* open_movie_acquire_next_frame(open_movie_mdec_stream_state_t* state) {
    void* frame_data;
    open_movie_frame_header_t* header;
    s32 attempts;

    attempts = 0;
    while (StGetNext(&frame_data, (void**)&header) != 0) {
        VSync(0);
        if (attempts == OPEN_MOVIE_ACQUIRE_MAX_ATTEMPTS) {
            return 0;
        }
        attempts++;
    }

    g_open_movie_current_frame = header->frame_number;
    if (header->frame_number >= g_open_movie_end_frame) {
        g_open_movie_stream_status = OPEN_MOVIE_STREAM_STOP_AND_PAUSE_AUDIO;
    }
    if (g_open_movie_video_end_frame != -1 && header->frame_number >= g_open_movie_video_end_frame) {
        g_open_movie_stream_status = OPEN_MOVIE_STREAM_STOP;
    }

    if (g_open_movie_width != header->width || g_open_movie_height != header->height) {
        RECT clear_rect;

        clear_rect.x = 0;
        clear_rect.y = 0;
        clear_rect.w = 0x1e0;
        clear_rect.h = 0x1e0;
        ClearImage(&clear_rect, 0, 0, 0);
        g_open_movie_width = header->width;
        g_open_movie_height = header->height;
        g_open_movie_aligned_height = (header->height + 15) & 0xfff0;
    }

    state->frame_rects[0].w = state->frame_rects[1].w = g_open_movie_width * 3 / 2;
    state->frame_rects[0].h = state->frame_rects[1].h = g_open_movie_aligned_height;
    state->output_rect.h = g_open_movie_aligned_height;
    return frame_data;
}
