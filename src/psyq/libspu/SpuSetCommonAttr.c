/* SCUS_942.21 0x8001b094..0x8001b427; tables 0x8001008c..0x800100cb. */
#include "psx/libspu.h"

void SpuSetCommonAttr(SpuCommonAttr* attr) {
    u16 left = 0;
    /* Unpinned right volume shifts the mask/all temporaries to different registers. */
    register u16 right __asm__("$8") = 0;
    u32 mask = attr->mask;
    s32 all = mask == 0;
    u32 mode;
    /* Removing this dead local removes the retail 16-byte frame. */
    u32 scratch[4];
    s32 volume;
    /* Unpinned register pointer swaps v0/v1 throughout the four mixer controls. */
    register volatile psyq_spu_registers_t* registers __asm__("$2");
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
            default:
                left = *(u16*)&attr->mvol.left;
                mode = 0;
                break;
            }
        } else {
            left = *(u16*)&attr->mvol.left;
            mode = 0;
        }
        if (mode != 0) {
            /* Unpinned input removes the retail a3-to-v1 clamp copy. */
            register s32 requested __asm__("$7") = attr->mvol.left;
            volume = requested;
            if (requested >= 128) {
                left = 127;
            } else if (requested < 0) {
                left = 0;
            } else {
                left = volume;
            }
        }
        {
            u32 packed = left & 0x7fff;
            packed |= mode;
            _spu_RXX->main_volume_left = packed;
        }
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
            default:
                right = *(u16*)&attr->mvol.right;
                mode = 0;
                break;
            }
        } else {
            right = *(u16*)&attr->mvol.right;
            mode = 0;
        }
        if (mode != 0) {
            /* Unpinned input removes the retail a2-to-v1 clamp copy. */
            register s32 requested __asm__("$6") = attr->mvol.right;
            volume = requested;
            if (requested >= 128) {
                right = 127;
            } else if (requested < 0) {
                right = 0;
            } else {
                right = volume;
            }
        }
        {
            /* Unpinned packing clobbers the right-volume register and its branch delay. */
            register u32 packed __asm__("$2") = right & 0x7fff;
            packed |= mode;
            _spu_RXX->main_volume_right = packed;
        }
    }
    if (all || (mask & 0x40)) {
        _spu_RXX->cd_volume_left = *(u16*)&attr->cd.volume.left;
    }
    if (all || (mask & 0x80)) {
        _spu_RXX->cd_volume_right = *(u16*)&attr->cd.volume.right;
    }
    if (all || (mask & 0x400)) {
        _spu_RXX->external_volume_left = *(u16*)&attr->ext.volume.left;
    }
    if (all || (mask & 0x800)) {
        _spu_RXX->external_volume_right = *(u16*)&attr->ext.volume.right;
    }
    if (all || (mask & 0x100)) {
        if (attr->cd.reverb == 0) {
            registers = _spu_RXX;
            control = registers->control & 0xfffb;
        } else {
            registers = _spu_RXX;
            control = registers->control | 4;
        }
        registers->control = control;
    }
    if (all || (mask & 0x200)) {
        if (attr->cd.mix == 0) {
            registers = _spu_RXX;
            control = registers->control & 0xfffe;
        } else {
            registers = _spu_RXX;
            control = registers->control | 1;
        }
        registers->control = control;
    }
    if (all || (mask & 0x1000)) {
        if (attr->ext.reverb == 0) {
            registers = _spu_RXX;
            control = registers->control & 0xfff7;
        } else {
            registers = _spu_RXX;
            control = registers->control | 8;
        }
        registers->control = control;
    }
    if (all || (mask & 0x2000)) {
        if (attr->ext.mix == 0) {
            registers = _spu_RXX;
            control = registers->control & 0xfffd;
        } else {
            registers = _spu_RXX;
            control = registers->control | 2;
        }
        registers->control = control;
    }
}
