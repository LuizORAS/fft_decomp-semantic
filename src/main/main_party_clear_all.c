#include "fft/main.h"
#include "psx/types.h"

void main_party_clear_all(void) {
    s32 i;

    for (i = PARTY_ROSTER_SLOT_COUNT - 1; i >= 0; i--) {
        g_main_party_data[i].party_id = PARTY_ID_NONE;
    }
}
