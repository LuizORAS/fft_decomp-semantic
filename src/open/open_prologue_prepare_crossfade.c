#include "fft/open.h"

#define OPEN_IMAGE_COPY_WIDTH  210
#define OPEN_IMAGE_COPY_HEIGHT 180

/* View of the frame image at g_open_file_destination, 256 pixels per row:
 * the 210x180 picture starts at byte 0x5600, the first pixel of row 43. */
typedef struct open_gfx_image_file {
    u8 _unused_0000[0x5600];
    u16 pixels[1];
} open_gfx_image_file_t;

/* Prepare the crossfade of the new-game prologue's picture to OPNBK image
 * file_index + 2.
 *
 * Points the crossfade at its three buffers, starts the asynchronous read of the
 * new image into g_open_work_buffer_0 (the crossfade's source) and copies the
 * 210x180 picture on screen out of the frame image into g_open_work_buffer_1,
 * packed 210 pixels per row. It clears the per-row fade steps, the running flag
 * (row_flags[0]) and the progress row (row_flags[1], the same byte as
 * g_open_prologue_crossfade_progress); image operation 0x20 of a later prologue
 * page waits for the read and sets the running flag. */
void open_prologue_prepare_crossfade(s32 file_index) {
    u16* copy_source;
    u16* copy_destination;
    u16* file_pixels;
    s32 row;
    s32 source_row;
    s32 column;
    s32 source_offset;
    s32 destination_index;
    s32 source_index;
    u16* pixels;
    u16 value;
    open_gfx_image_file_t* file_image;

    copy_source = (u16*)g_open_work_buffer_0;
    copy_destination = (u16*)g_open_work_buffer_1;
    file_image = g_open_file_destination;
    file_pixels = file_image->pixels;
    g_open_prologue_crossfade_source = copy_source;
    g_open_prologue_crossfade_destination = copy_destination;
    g_open_prologue_crossfade_file_pixels = file_pixels;
    open_file_build_openbk_header(file_index + 2, g_open_work_buffer_0);

    row = 0;
    source_row = 0;
    do {
        column = 0;
        source_offset = row << 8;
        do {
            destination_index = source_row + column;
            source_index = source_offset + column;
            column++;
            destination_index <<= 1;
            pixels = g_open_prologue_crossfade_file_pixels;
            source_index <<= 1;
            /* index + (u32)pointer: the retail addu sums index first. */
            value = *(u16*)(source_index + (u32)pixels);
            *(u16*)(destination_index + (u32)g_open_prologue_crossfade_destination) = value;
        } while (column < OPEN_IMAGE_COPY_WIDTH);
        g_open_prologue_crossfade_row_flags[row + 2] = 0;
        row++;
        source_row += OPEN_IMAGE_COPY_WIDTH;
    } while (row < OPEN_IMAGE_COPY_HEIGHT);

    g_open_prologue_crossfade_row_flags[0] = 0;
    g_open_prologue_crossfade_row_flags[1] = 0;
}
