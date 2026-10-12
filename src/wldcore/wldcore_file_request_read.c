#include "fft/wldcore.h"

/* Wait for the shared disc read to finish (wldcore_wait_for_file_load), then start the next one
 * without the Now Loading message (main_file_request_read_quiet). */
void wldcore_file_request_read(main_file_load_descriptor_t* header, s32 sector, s32 sectors, void* destination) {
    wldcore_wait_for_file_load();
    main_file_request_read_quiet(header, sector, sectors, destination);
}
