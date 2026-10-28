#include "psx/suzuki.h"
#include "psx/types.h"

/* Set the state of suzuki_smd_random. Nothing calls it: suzuki_sound_init writes its seed itself. */
void suzuki_smd_seed_random(s32 value) {
    g_suzuki_smd_random_state = value;
}
