#include "fft/main.h"

/* Same as main_file_alloc_and_load, reporting error 2 when it returns 0. Graphics loaders use it: the
 * zodiac frame, FRAME.BIN, EFC_FNT, WLDFACE and other WORLD images. */
void* main_file_alloc_and_load_checked(int sector, int size) {
    void* result = main_file_alloc_and_load(sector, size);

    if (result == 0) {
        main_system_handle_animation_exception(2);
    }
    return result;
}
