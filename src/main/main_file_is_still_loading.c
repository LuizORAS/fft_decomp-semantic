#include "fft/main.h"

/* Return the shared descriptor's state: nonzero while its read is in flight. */
int main_file_is_still_loading(void) {
    return g_main_file_still_loading;
}
