#include "psx/suzuki.h"
#include "psx/types.h"

void suzuki_smd_seed_random(s32 value) {
    g_suzuki_smd_random_state = value;
}
