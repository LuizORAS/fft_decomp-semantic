#include "fft/battle_effect.h"

/* Return the original component with the largest signed-16 magnitude.
 *
 * Strict comparisons make earlier arguments win ties. */
s16 battle_effect_select_largest_magnitude_s16(s16 first, s16 second, s16 third) {
    s16 first_magnitude;
    s16 second_magnitude;
    s16 third_magnitude;
    s16 result;

    first_magnitude = first < 0 ? -first : first;
    second_magnitude = second < 0 ? -second : second;
    third_magnitude = third < 0 ? -third : third;

    if (first_magnitude < second_magnitude) {
        result = second;
        if (second_magnitude < third_magnitude) {
            result = third;
        }
    } else {
        result = first;
        if (first_magnitude < third_magnitude) {
            result = third;
        }
    }
    return result;
}
