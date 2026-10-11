#include "fft/main.h"

/* Roll a party record's raw stats for unit_type (main_unit_generate_base_raw_stats). */
void main_unit_generate_party_base_raw_stats(party_data_t* party, s32 unit_type) {
    main_unit_generate_base_raw_stats(party->raw_stats, unit_type);
}
