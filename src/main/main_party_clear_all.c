#include "fft/main.h"
#include "psx/types.h"

/* Empty the roster: every slot's party_id becomes PARTY_ID_NONE. */
void main_party_clear_all(void) {
    u8 value = PARTY_ID_NONE;
    s32 i = PARTY_ROSTER_SLOT_COUNT - 1;

    do {
        g_main_party_data[i].party_id = value;
        i--;
    } while (i >= 0);
}
