#include "fft/open.h"
#include "psx/types.h"

void open_sound_set_cd_mix_and_volume(s32 cd_mix_level, s32 duration) {
    s32 volume;

    if (cd_mix_level == 0) {
        volume = 0;
    } else {
        main_sound_put_type(cd_mix_level);
        volume = 0x6400;
    }

    main_sound_set_cd_volume(volume, duration);
}
