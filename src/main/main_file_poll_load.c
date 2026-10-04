#include "fft/main.h"
#include "psx/libcd.h"

/* Advance a CD read by one step per call, once per frame: set double speed, wait 4 frames,
 * seek, read. A disc error, or a read that after 256 frames has read nothing or reports an
 * error, resets the drive and starts over (error_count counts the retries). Draws the Now
 * Loading message while the read is busy.
 *
 * Sequential transitions use ++ because explicit enum assignments replace the
 * target's increment instructions and do not match. */
void main_file_poll_load(main_file_load_descriptor_t* descriptor) {
    u8 mode;
    s32 result;

    switch (descriptor->state) {
    case MAIN_FILE_LOAD_STATE_IDLE:
        break;
    case MAIN_FILE_LOAD_STATE_SET_DOUBLE_SPEED:
        mode = CdlModeSpeed;
        CdControl(CdlSetmode, &mode, 0);
        g_main_system_frame_timer = 0;
        descriptor->state++;
        break;
    case MAIN_FILE_LOAD_STATE_WAIT_AFTER_SET_MODE:
        if (g_main_system_frame_timer >= 4) {
            descriptor->state++;
        }
        break;
    case MAIN_FILE_LOAD_STATE_START_SEEK:
        CdControlF(CdlSeekL, descriptor->position);
        descriptor->state++;
        break;
    case MAIN_FILE_LOAD_STATE_POLL_SEEK:
        result = CdSync(1, 0);
        if (result == CdlComplete) {
            descriptor->state++;
            if (descriptor->destination == 0) {
                descriptor->state = MAIN_FILE_LOAD_STATE_IDLE;
            }
        } else if (result == CdlDiskError) {
            main_file_reset_cd_subsystems();
            descriptor->state = MAIN_FILE_LOAD_STATE_SET_DOUBLE_SPEED;
            descriptor->error_count = descriptor->error_count + 1;
        }
        break;
    case MAIN_FILE_LOAD_STATE_START_READ:
        if (CdRead(descriptor->sector_count, (u32*)descriptor->destination, CdlModeSpeed) == 0) {
            descriptor->error_count = descriptor->error_count + 1;
        } else {
            g_main_system_frame_timer = 0;
            descriptor->wait_frames = 0;
            descriptor->state++;
        }
        break;
    case MAIN_FILE_LOAD_STATE_POLL_READ:
        result = CdReadSync(1, 0);
        if (result == 0) {
            descriptor->state = MAIN_FILE_LOAD_STATE_IDLE;
        } else if (result == descriptor->sector_count || result == -1) {
            if (g_main_system_frame_timer > 0x100) {
                g_main_system_frame_timer = 0;
                descriptor->wait_frames = 0;
                main_file_reset_cd_subsystems();
                descriptor->state = MAIN_FILE_LOAD_STATE_SET_DOUBLE_SPEED;
                descriptor->error_count = descriptor->error_count + 1;
            } else {
                descriptor->wait_frames = descriptor->wait_frames + 1;
            }
        } else {
            descriptor->wait_frames = 0;
        }
        break;
    }
    if (descriptor->state != MAIN_FILE_LOAD_STATE_IDLE) {
        main_gfx_draw_now_loading_message();
    }
}
