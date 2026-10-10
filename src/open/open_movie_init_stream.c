#include "fft/open.h"
#include "psx/libcd.h"
#include "psx/types.h"

/* Prepare the MDEC and the 0x30-sector CD ring for a movie and aim the drive at
 * it: a one-sector read with no destination makes the file loader seek to
 * `sector` and leaves its position in g_open_file_location (the loader
 * descriptor's position field), which is then set as the read location. */
void open_movie_init_stream(s32 sector, void* output_callback) {
    main_file_load_descriptor_t* header;

    DecDCTReset(0);
    DecDCToutCallback(output_callback);
    StSetRing(g_open_movie_cd_stream_ring_buffer, 0x30);
    StSetStream(1, 0, -1, 0, 0);

    header = &g_main_file_cd_state;
    main_file_build_header_nnl(header, sector, 1, 0);
    while (g_main_file_still_loading != 0) {
        main_file_poll_load(header);
        VSync(0);
    }

    while (CdControl(CdlSetloc, g_open_file_location, 0) == 0) { }
}
