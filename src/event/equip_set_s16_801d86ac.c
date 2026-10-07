#include "fft/event_equip.h"
#include "psx/types.h"

/* Stores value in D_801d86ac. Nothing on the disc calls it. */
void equip_set_s16_801d86ac(s32 value) {
    D_801d86ac = value;
}
