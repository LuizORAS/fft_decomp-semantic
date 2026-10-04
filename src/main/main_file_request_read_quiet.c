#include "fft/main.h"

/* Same as main_file_request_read with display mode -1: no Now Loading message. */
int main_file_request_read_quiet(main_file_load_descriptor_t* descriptor, int sector, int sectors, void* destination) {
    return main_file_request_read(descriptor, sector, sectors, destination, -1);
}
