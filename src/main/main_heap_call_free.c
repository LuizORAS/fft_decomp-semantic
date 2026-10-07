#include "fft/main.h"

/* Call main_heap_free and drop its result. */
void main_heap_call_free(void* allocation) {
    main_heap_free(allocation);
}
