#include "fft/main.h"

/* Empty error hook. Callers pass an id of the reporting subsystem and a code;
 * both are dropped. */
void main_system_handle_malloc_exception(s32 allocator_id, s32 error_code) {
}
