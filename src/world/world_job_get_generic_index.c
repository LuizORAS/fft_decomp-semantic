#include "fft/world.h"
#include "psx/types.h"

/* Map a generic job id (Squire..Mime) to its 0-based index; anything else is 0.
 *
 * Callers pass the id as a full word and `id` narrows it, which also keeps the
 * compare and the assignment separate subtractions. */
s32 world_job_get_generic_index(s32 job_id) {
    u16 id = job_id;
    s16 index;

    if ((u16)(id - JOB_ID_SQUIRE) < JOB_ID_GENERIC_COUNT) {
        index = id;
        index -= JOB_ID_SQUIRE;
    } else {
        index = 0;
    }
    return index;
}
