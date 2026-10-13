#include "fft/main.h"

party_unit_t* main_party_get_unit(s32 party_index) {
    return party_index < PARTY_ROSTER_SLOT_COUNT ? &g_main_party_units[party_index] : 0;
}
