#include "fft/world.h"
#include "psx/types.h"

/* Map a generic job id (Squire..Mime) to its 0-based index; anything else is 0.
 *
 * Callers pass the id as a full 32-bit value and `id` narrows it, which also keeps the
 * compare and the assignment separate subtractions. */
s32 world_job_get_generic_index(s32 job_id) {
    u16 id = job_id;
    s16 index;

    if (id >= JOB_ID_SQUIRE && id <= JOB_ID_MIME) {
        index = id;
        index -= JOB_ID_SQUIRE;
    } else {
        index = 0;
    }
    return index;
}
