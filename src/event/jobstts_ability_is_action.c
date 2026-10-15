#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Menu callback: 1 when the cached ability type is ABILITY_TYPE_DEFAULT, an
 * ability with a range record. */
s32 jobstts_ability_is_action(s32 unused_row) {
    return g_jobstts_ability_type == ABILITY_TYPE_DEFAULT;
}
