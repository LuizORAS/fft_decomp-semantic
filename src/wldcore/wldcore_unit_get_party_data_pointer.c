#include "fft/wldcore.h"

/* Same as main_party_get_data_pointer: WLDCORE's proposition and roster code reads party records
 * through it. */
party_data_t* wldcore_unit_get_party_data_pointer(s32 party_index) {
    return main_party_get_data_pointer(party_index);
}
