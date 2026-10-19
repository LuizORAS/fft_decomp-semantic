#include "fft/wldcore.h"

/* Same as main_party_get_unit: WLDCORE's proposition and roster code reads party records
 * through it. */
party_unit_t* wldcore_unit_get_party_unit(s32 party_index) {
    return main_party_get_unit(party_index);
}
