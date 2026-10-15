/* SCUS_942.21 0x8001b094..0x8001b427; tables 0x8001008c..0x800100cb. */
#include "psx/libspu.h"

/* Set the SPU's common attributes that `attr`'s mask selects (all for mask 0): the main volume, fixed or a
 * sweep (modes 1-7, the sweep value clamped to 0-127), the CD and external input volumes, and whether each
 * input is mixed in and reverberated. main_sound_root_counter_2_handler and main_sound_set_cd_reverb call
 * it. */
void SpuSetCommonAttr(SpuCommonAttr* attr) {
    s16 left = 0;
    s16 right = 0;
    u32 mask = attr->mask;
    s32 all = mask == 0;
    u32 mode;
    u16 control;
    if (all || (mask & 1)) {
        if (all || (mask & 4)) {
            switch (attr->mvolmode.left) {
            case 1:
                mode = 0x8000;
                break;
            case 2:
                mode = 0x9000;
                break;
            case 3:
                mode = 0xa000;
                break;
            case 4:
                mode = 0xb000;
                break;
            case 5:
                mode = 0xc000;
                break;
            case 6:
                mode = 0xd000;
                break;
            case 7:
                mode = 0xe000;
                break;
            case 0:
                left = attr->mvol.left;
                mode = 0;
                break;
            default:
                left = attr->mvol.left;
                mode = 0;
                break;
            }
        } else {
            left = attr->mvol.left;
            mode = 0;
        }
        if (mode != 0) {
            if (attr->mvol.left >= 128) {
                left = 127;
            } else if (attr->mvol.left < 0) {
                left = 0;
            } else {
                left = attr->mvol.left;
            }
        }
        _spu_RXX->rxx.main_volume_left = (left & 0x7fff) | mode;
    }
    if (all || (mask & 2)) {
        if (all || (mask & 8)) {
            switch (attr->mvolmode.right) {
            case 1:
                mode = 0x8000;
                break;
            case 2:
                mode = 0x9000;
                break;
            case 3:
                mode = 0xa000;
                break;
            case 4:
                mode = 0xb000;
                break;
            case 5:
                mode = 0xc000;
                break;
            case 6:
                mode = 0xd000;
                break;
            case 7:
                mode = 0xe000;
                break;
            case 0:
                right = attr->mvol.right;
                mode = 0;
                break;
            default:
                right = attr->mvol.right;
                mode = 0;
                break;
            }
        } else {
            right = attr->mvol.right;
            mode = 0;
        }
        if (mode != 0) {
            if (attr->mvol.right >= 128) {
                right = 127;
            } else if (attr->mvol.right < 0) {
                right = 0;
            } else {
                right = attr->mvol.right;
            }
        }
        _spu_RXX->rxx.main_volume_right = (right & 0x7fff) | mode;
    }
    if (all || (mask & 0x40)) {
        _spu_RXX->rxx.cd_volume_left = attr->cd.volume.left;
    }
    if (all || (mask & 0x80)) {
        _spu_RXX->rxx.cd_volume_right = attr->cd.volume.right;
    }
    if (all || (mask & 0x400)) {
        _spu_RXX->rxx.external_volume_left = attr->ext.volume.left;
    }
    if (all || (mask & 0x800)) {
        _spu_RXX->rxx.external_volume_right = attr->ext.volume.right;
    }
    if (all || (mask & 0x100)) {
        if (attr->cd.reverb == 0) {
            control = _spu_RXX->rxx.control;
            control &= 0xfffb;
            _spu_RXX->rxx.control = control;
        } else {
            control = _spu_RXX->rxx.control;
            control |= 4;
            _spu_RXX->rxx.control = control;
        }
    }
    if (all || (mask & 0x200)) {
        if (attr->cd.mix == 0) {
            control = _spu_RXX->rxx.control;
            control &= 0xfffe;
            _spu_RXX->rxx.control = control;
        } else {
            control = _spu_RXX->rxx.control;
            control |= 1;
            _spu_RXX->rxx.control = control;
        }
    }
    if (all || (mask & 0x1000)) {
        if (attr->ext.reverb == 0) {
            control = _spu_RXX->rxx.control;
            control &= 0xfff7;
            _spu_RXX->rxx.control = control;
        } else {
            control = _spu_RXX->rxx.control;
            control |= 8;
            _spu_RXX->rxx.control = control;
        }
    }
    if (all || (mask & 0x2000)) {
        if (attr->ext.mix == 0) {
            control = _spu_RXX->rxx.control;
            control &= 0xfffd;
            _spu_RXX->rxx.control = control;
        } else {
            control = _spu_RXX->rxx.control;
            control |= 2;
            _spu_RXX->rxx.control = control;
        }
    }
}
