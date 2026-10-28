#include "fft/main.h"

/* Same as main_file_load_to_address, reporting error 2 through main_system_handle_error
 * for a null destination. */
void* main_file_load_to_address_checked(int sector, int size, void* destination) {
    void* result = main_file_load_to_address(sector, size, destination);

    if (result == 0) {
        main_system_handle_error(MAIN_SYSTEM_ERROR_FILE_LOAD);
    }
    return result;
}
