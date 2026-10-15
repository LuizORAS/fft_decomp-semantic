#include "fft/main.h"
#include "psx/types.h"

/* Empty roster slot index (party_id PARTY_ID_NONE); the record's other bytes stay. */
void main_party_remove_unit(u32 index) {
    g_main_party_data[index].party_id = PARTY_ID_NONE;
}
