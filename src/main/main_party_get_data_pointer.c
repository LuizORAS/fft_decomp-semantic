#include "fft/main.h"

party_data_t* main_party_get_data_pointer(s32 party_index) {
    return party_index < PARTY_ROSTER_SLOT_COUNT ? &g_main_party_data[party_index] : 0;
}
