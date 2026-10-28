#include "fft/main.h"

/* Roll a party record's raw stats for unit_type (main_unit_generate_base_raw_stats). */
void main_unit_generate_party_base_raw_stats(party_unit_t* party, main_unit_generation_type_e unit_type) {
    main_unit_generate_base_raw_stats(party->raw_stats, unit_type);
}
