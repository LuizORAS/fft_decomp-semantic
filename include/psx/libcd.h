#ifndef PSX_LIBCD_H
#define PSX_LIBCD_H

#include "psx/libapi.h"
#include "psx/libspu.h"

#include "psx/types.h"

#define CdlNop        0x01
#define CdlSetloc     0x02
#define CdlReadN      0x06
#define CdlPause      0x09
#define CdlReset      0x0a
#define CdlDemute     0x0c
#define CdlSetmode    0x0e
#define CdlSeekL      0x15
#define CdlReadS      0x1b
#define CdlModeSize0  0x10
#define CdlModeSize1  0x20
#define CdlModeSpeed  0x80
#define CdlModeStream 0x100

#define CdlStatShellOpen 0x10
#define CdlStatRead      0x20
#define CdlStatSeek      0x40
#define CdlStatPlay      0x80

#define CdlDataReady   0x01
#define CdlComplete    0x02
#define CdlAcknowledge 0x03
#define CdlDataEnd     0x04
#define CdlDiskError   0x05

typedef enum {
    PSYQ_CD_SECTORS_PER_SECOND = 75,
    PSYQ_CD_SECONDS_PER_MINUTE = 60,
    PSYQ_CD_LEAD_IN_SECTORS = 150,
} psyq_cd_position_unit_e;

typedef enum {
    StFREE = 0,
    StREWIND = 1,
    StCOMPLETE = 2,
    StBUSY = 3,
    StLOCK = 4,
} psyq_cd_stream_status_e;

/* BCD minute/second/sector position; the encoder leaves track unchanged. */
typedef struct {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
} CdlLOC;

/* Psy-Q CdlATV: CD audio to SPU attenuation (left to left, left to right,
 * right to right, right to left). */
typedef struct {
    u8 val0;
    u8 val1;
    u8 val2;
    u8 val3;
} CdlATV;

extern s32 CdInit(void);
extern int CdSetDebug(int level);
extern void SetMem(int);
extern void* CdReadyCallback(void* callback);
extern void* CdReadCallback(void* callback);
extern int CdControl(u8, u8*, u8*);
extern int CdControlF(u8, u8*);
extern int CdSync(int, u8*);
extern int CdRead(int, u32*, int);
extern int CdReadSync(int, u8*);
extern int CdReady(int, u8*);
extern int CdGetSector(void*, int);
extern int CdDataSync(int);

s32 CdMix(CdlATV* vol);
s32 CdControlB(s32 command, const u8* parameter, u8* result);
s32 CdRead2(s32 mode);
s32 StGetBackloc(CdlLOC* position);
s32 StGetNext(void** frame_data, void** header);
void CdFlush(void);
CdlLOC* CdIntToPos(s32 sector, CdlLOC* position);
s32 CdReset(s32 mode);
void StCdInterrupt(void);
s32 StFreeRing(void* frame_data);
void StRingStatus(s16* free_sectors, s16* read_sectors);
void StUnSetRing(void);
void StSetRing(void* buffer, s32 sectors);
void StSetStream(s32 mode, s32 start_frame, s32 end_frame, void* frame_callback, void* end_callback);
void StSetMask(s32 mask, s32 start_frame, s32 end_frame);
s32 CdPosToInt(CdlLOC* position);
char* CdComstr(u8 command);
char* CdIntstr(u8 interrupt);
s32 CdStatus(void);
s32 CdMode(void);
s32 CdLastCom(void);
CdlLOC* CdLastPos(void);
void* CdSyncCallback(void* callback);

void* CdDataCallback(void* callback);
void StClearRing(void);

/* driver internals */

typedef enum {
    PSYQ_CD_DATA_WORDS = 2048 / sizeof(u32),
    PSYQ_CD_SECTOR_2340_WORDS = 2340 / sizeof(u32),
    PSYQ_CD_SECTOR_2328_WORDS = 2328 / sizeof(u32),
    PSYQ_CD_READ_TIMEOUT_VBLANKS = 1200,
    PSYQ_CD_SECTOR_TIMEOUT_VBLANKS = 60,
    PSYQ_CD_STREAM_HEADER_WORDS = 8,
    PSYQ_CD_STREAM_PAYLOAD_WORDS = 504,
    PSYQ_CD_STREAM_PAYLOAD_BYTES = PSYQ_CD_STREAM_PAYLOAD_WORDS * sizeof(u32),
    PSYQ_CD_STREAM_HEADER_MAGIC = 0x160,
    PSYQ_CD_DMA_CHANNEL = 3,
} psyq_cd_read_control_e;

/* Resident LIBCD state, reconstructed from the original register accesses. */
extern u8 g_psyq_cd_status;
extern u8 g_psyq_cd_mode;
extern u8 g_psyq_cd_last_command;
extern u8 g_psyq_cd_last_position[4];

extern char* g_psyq_cd_command_names[28];
extern char* g_psyq_cd_interrupt_names[7];
extern char g_psyq_cd_unknown_name[];
extern s32 g_psyq_cd_debug_level;
extern void* g_psyq_cd_sync_callback;
extern void* g_psyq_cd_ready_callback;
extern void* g_psyq_cd_read_callback;
void def_cbsync(void);
void def_cbready(void);
void def_cbread(void);
void CD_flush(void);
s32 CD_vol(CdlATV* volume);
s32 CD_getsector(void* destination, s32 words);
s32 CD_datasync(s32 mode);
s32 CD_sync(s32 mode, u8* result);
s32 CD_ready(s32 mode, u8* result);
void psyq_cd_stream_ready_callback(void);

extern s32 g_psyq_cd_stream_ring_sectors;
extern s32 g_psyq_cd_test_parameter_count;
extern char g_psyq_cd_init_failure_message[];
void CD_initintr(void);
s32 CD_init(void);
s32 CD_initvol(void);
void CD_set_test_parmnum(s32 mode);

/* Current SDK caller ABI corrections, established by original v0 use. */
s32 CD_cw(u8 command, const u8* parameter, u8* result, s32 asynchronous);
void callback(void);
void init_ring_status(u32 first_sector, u32 sector_count);
void mem2mem(u32* destination, u32* source, u32 words);

/* Original data pointers (typed volatile at hardware register accesses). */
extern volatile u8* g_psyq_cd_index_status_register;
extern volatile u8* g_psyq_cd_command_response_register;
extern volatile u8* g_psyq_cd_parameter_data_register;
extern volatile u8* g_psyq_cd_request_interrupt_register;
extern volatile u32* g_psyq_cd_common_delay_register;
extern volatile psyq_spu_registers_t* g_psyq_cd_spu_registers;
extern volatile u8* g_psyq_cd_unset_index_register;
extern volatile u8* g_psyq_cd_unset_request_register;
extern u32 g_psyq_cd_status_detail;
extern char g_psyq_cd_init_message[];
extern char g_psyq_cd_init_address_format[];
extern u8 g_psyq_cd_sync_interrupt;
extern volatile u8 g_psyq_cd_ready_interrupt;        /* CD IRQ updates this result byte. */
extern volatile u8 g_psyq_cd_second_ready_interrupt; /* CD IRQ updates this result byte. */

/* Stream ring records, corroborated by StHEADER and the retail IRQ/callback accesses. */
typedef struct {
    u16 status;
    u16 type;
    u16 sector_index;
    u16 sector_count;
    u32 frame_count;
    u32 frame_size;
    u32 dimensions;
    u32 dummy1;
    u32 dummy2;
    CdlLOC position;
} psyq_cd_ring_record_t;
extern psyq_cd_ring_record_t* volatile g_psyq_cd_stream_ring; /* Streaming IRQ updates the ring base. */
extern u32 g_psyq_cd_stream_write_index;
extern u32 g_psyq_cd_stream_frame_start_index;
extern u32 g_psyq_cd_stream_read_index;
extern s32 g_psyq_cd_stream_emulation_base;
extern s32 g_psyq_cd_stream_end_frame;
extern s32 g_psyq_cd_stream_start_mask;
extern s16 g_psyq_cd_stream_expected_sector_index;
extern s32 g_psyq_cd_stream_frame_count;
extern s32 g_psyq_cd_stream_mode;
extern s32 g_psyq_cd_stream_skip_sector_position;
extern void (*g_psyq_cd_stream_frame_callback)(void);
extern void (*g_psyq_cd_stream_end_callback)(void);
extern s32 g_psyq_cd_stream_interrupt_pending;
extern s32 g_psyq_cd_stream_requested_channel;
extern s32 g_psyq_cd_stream_start_frame;
extern s32 g_psyq_cd_stream_current_channel;
extern s32 g_psyq_cd_stream_frame_dma_pending;
extern CdlLOC g_psyq_cd_backloc_position;
extern u32 g_psyq_cd_backloc_frame;

/* Remaining low-level CD register aliases and command table. */
extern s32 g_psyq_cd_command_uses_position[32];
extern volatile u32* g_psyq_cd_access_delay_register; /* CD access delay: 1f801018 */
extern volatile u32* g_psyq_cd_dma_priority_register; /* DMA priority: 1f8010f0 */
extern volatile u32* g_psyq_cd_dma_address_register;  /* DMA3 MADR */
extern volatile u32* g_psyq_cd_dma_block_register;    /* DMA3 BCR */
extern volatile u32* g_psyq_cd_dma_control_register;  /* DMA3 CHCR */
s32 getintr(void);
extern u8 g_psyq_cd_sync_result[8];
extern u8 g_psyq_cd_ready_result[8];
extern s32 g_psyq_cd_timeout_deadline;
extern s32 g_psyq_cd_timeout_poll_count;
extern char* g_psyq_cd_timeout_description;
extern char g_psyq_cd_timeout_message[];
extern char g_psyq_cd_timeout_format[];
extern char g_psyq_cd_data_sync_timeout_name[];
void data_ready_callback(void);
extern u8* g_psyq_cd_init_irq_state_pointer;

/* CD command attribute tables (remain original data). */
extern s32 g_psyq_cd_command_waits_for_complete[32];
extern s32 g_psyq_cd_command_resets_ready[32];
extern s32 g_psyq_cd_command_ack_updates_status[32];
extern s32 g_psyq_cd_command_parameter_counts[32];
extern s32 g_psyq_cd_shell_open_count;
extern u8 g_psyq_cd_second_ready_result[8];
extern char g_psyq_cd_disk_error_message[];
extern char g_psyq_cd_disk_error_format[];
extern char g_psyq_cd_unknown_interrupt_message[];
extern char g_psyq_cd_unknown_interrupt_format[];
extern char g_psyq_cd_sync_timeout_name[];
extern char g_psyq_cd_ready_timeout_name[];
extern char g_psyq_cd_command_trace_format[];
extern char g_psyq_cd_missing_parameters_format[];
extern char g_psyq_cd_command_timeout_name[];

/* Every original polling copy uses this byte/byte result contract. */
typedef void (*psyq_cd_result_callback_t)(u8 event, u8* result);
typedef struct {
    u8 sync;
    u8 ready;
    u8 second_ready;
} psyq_cd_interrupt_result_t;
extern volatile psyq_cd_interrupt_result_t g_psyq_cd_irq_state;
/* g_psyq_cd_ready_interrupt also has a measured store/reload in getintr. */

typedef struct {
    s32 sectors;
    u32* first_buffer;
    u32* next_buffer;
    s32 mode;
    s32 words;
    s32 remaining;
    s32 sector_clock;
    s32 start_clock;
    s32 expected_sector;
    void* sync_callback;
    void* ready_callback;
} psyq_cd_read_state_t;
extern volatile psyq_cd_read_state_t g_psyq_cd_read_state;

/* Separate aliases retain measured independent address reloads. */
extern s32 g_psyq_cd_read_sectors;
extern u32* g_psyq_cd_read_first_buffer;
extern u32* g_psyq_cd_read_next_buffer;
extern volatile s32 g_psyq_cd_read_mode;
extern s32 g_psyq_cd_read_words;
extern volatile s32 g_psyq_cd_read_remaining;
extern s32 g_psyq_cd_read_sector_clock;
extern s32 g_psyq_cd_read_start_clock;
extern s32 g_psyq_cd_read_expected_sector;
extern void* g_psyq_cd_read_saved_sync_callback;
extern void* g_psyq_cd_read_saved_ready_callback;
void cb_read(u8 event, u8* result);
s32 cd_read_retry(s32 retry);
extern char g_psyq_cd_sector_error_message[];
extern char g_psyq_cd_shell_open_message[];
extern char g_psyq_cd_read_retry_message[];

/* Streaming controller aliases, bound to original pointer words. */
extern volatile u8* g_psyq_cd_stream_index_register;
extern volatile u8* g_psyq_cd_stream_data_register;
extern volatile u8* g_psyq_cd_stream_request_register;
extern volatile u32* g_psyq_cd_stream_access_delay_register;
extern volatile u32* g_psyq_cd_stream_common_delay_register;
extern volatile u32* g_psyq_cd_stream_dma_priority_register;
typedef union {
    u32 word;
    u8 bytes[4];
} psyq_cd_dma_interrupt_register_t;
extern volatile psyq_cd_dma_interrupt_register_t* g_psyq_cd_stream_dma_interrupt_register;
extern volatile u32* g_psyq_cd_stream_mdec_out_dma_control_register;
extern volatile u32* g_psyq_cd_stream_dma_control_register;
extern s32 g_psyq_cd_stream_diagnostic_code;
extern u32 g_psyq_cd_stream_emulation_sector_index;
extern u32* g_psyq_cd_stream_payload;
extern psyq_cd_ring_record_t* g_psyq_cd_stream_current_header;
extern char g_psyq_cd_dma_timeout_format[];
void dma_execute(s32 channel, u32* destination, u32 blocks, u32 words, u32 control, u8 enable_irq, s32 unused);

/* Direct DMA channels occupy 16 bytes each at 0x1f801080. */
typedef struct {
    u32 address;
    u32 block;
    u32 control;
    u32 _padding_0c; /* unused fourth register slot in each channel */
} psyq_cd_dma_channel_t;

/* LIBCD Alarm record, 12 bytes at 0x80032a94.
 * Command waits and streaming interrupt callbacks share this state.
 * The wait helpers use a nonvolatile view to preserve retail scheduling.
 */
typedef struct {
    s32 deadline;
    s32 counter;
    char* description;
} psyq_cd_timeout_state_t;
extern volatile psyq_cd_timeout_state_t g_psyq_cd_timeout;

/* Original completed-location/frame record, eight bytes at 0x80032aa0.
 * Position is copied by StGetBackloc; its adjacent frame word is returned.
 * Keep the old position/frame aliases for already-matched fresh-address loads.
 */
typedef struct {
    CdlLOC position;
    u32 frame;
} psyq_cd_backloc_state_t;
extern psyq_cd_backloc_state_t g_psyq_cd_backloc;

#endif
