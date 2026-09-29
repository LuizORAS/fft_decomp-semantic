#ifndef PSX_LIBGPU_H
#define PSX_LIBGPU_H

#include "psx/libapi.h"
#include "psx/libetc.h"
#include "psx/libgte.h"

#include "psx/types.h"

/* Command byte in the first GP0 word; packet lengths exclude the DMA tag. */
typedef enum {
    PSYQ_GPU_CODE_CLEAR_CACHE = 0x01,
    PSYQ_GPU_CODE_BLOCK_FILL = 0x02,
    PSYQ_GPU_CODE_POLY_F3 = 0x20,
    PSYQ_GPU_CODE_POLY_FT3 = 0x24,
    PSYQ_GPU_CODE_POLY_F4 = 0x28,
    PSYQ_GPU_CODE_POLY_FT4 = 0x2c,
    PSYQ_GPU_CODE_POLY_G3 = 0x30,
    PSYQ_GPU_CODE_POLY_GT3 = 0x34,
    PSYQ_GPU_CODE_POLY_G4 = 0x38,
    PSYQ_GPU_CODE_POLY_GT4 = 0x3c,
    PSYQ_GPU_CODE_LINE_F2 = 0x40,
    PSYQ_GPU_CODE_LINE_F3 = 0x48,
    PSYQ_GPU_CODE_LINE_F4 = 0x4c,
    PSYQ_GPU_CODE_LINE_G2 = 0x50,
    PSYQ_GPU_CODE_LINE_G3 = 0x58,
    PSYQ_GPU_CODE_LINE_G4 = 0x5c,
    PSYQ_GPU_CODE_TILE = 0x60,
    PSYQ_GPU_CODE_SPRT = 0x64,
    PSYQ_GPU_CODE_TILE1 = 0x68,
    PSYQ_GPU_CODE_TILE8 = 0x70,
    PSYQ_GPU_CODE_SPRT8 = 0x74,
    PSYQ_GPU_CODE_TILE16 = 0x78,
    PSYQ_GPU_CODE_SPRT16 = 0x7c,
    PSYQ_GPU_CODE_MOVE_IMAGE = 0x80,
} psyq_gpu_primitive_code_e;

typedef enum {
    PSYQ_GPU_CODE_RAW_TEXTURE = 0x01,
    PSYQ_GPU_CODE_SEMI_TRANSPARENT = 0x02,
} psyq_gpu_primitive_flag_e;

#define PSYQ_GPU_FIXED_TILE_WORDS          2
#define PSYQ_GPU_FIXED_SPRITE_WORDS        3
#define PSYQ_GPU_PACKET_WORDS(packet_type) (sizeof(packet_type) / sizeof(u32) - 1)
#define PSYQ_GPU_COMMAND_WORD(code)        ((u32)(code) << 24)
#define PSYQ_GPU_POLYLINE_END              0x55555555U

/* GP0 command bytes, used with PSYQ_GPU_COMMAND_WORD. */
typedef enum {
    PSYQ_GPU_CODE_CPU_TO_VRAM = 0xa0,
    PSYQ_GPU_CODE_VRAM_TO_CPU = 0xc0,
    PSYQ_GPU_CODE_DRAW_MODE = 0xe1,
    PSYQ_GPU_CODE_TEXTURE_WINDOW = 0xe2,
    PSYQ_GPU_CODE_DRAW_AREA_TOP_LEFT = 0xe3,
    PSYQ_GPU_CODE_DRAW_AREA_BOTTOM_RIGHT = 0xe4,
    PSYQ_GPU_CODE_DRAW_OFFSET = 0xe5,
    PSYQ_GPU_CODE_DRAW_MASK = 0xe6
} psyq_gpu_attribute_code_e;

/* GP1 command bytes; the payload occupies the low 24 bits. */
typedef enum {
    PSYQ_GPU_GP1_RESET = 0x00,
    PSYQ_GPU_GP1_RESET_BUFFER = 0x01,
    PSYQ_GPU_GP1_ACK_IRQ = 0x02,
    PSYQ_GPU_GP1_DISPLAY_ENABLE = 0x03,
    PSYQ_GPU_GP1_DMA_DIRECTION = 0x04,
    PSYQ_GPU_GP1_DISPLAY_START = 0x05,
    PSYQ_GPU_GP1_HORIZONTAL_RANGE = 0x06,
    PSYQ_GPU_GP1_VERTICAL_RANGE = 0x07,
    PSYQ_GPU_GP1_DISPLAY_MODE = 0x08,
    PSYQ_GPU_GP1_VRAM_SIZE = 0x09,
    PSYQ_GPU_GP1_GET_INFO = 0x10,
    PSYQ_GPU_GP1_VRAM_SIZE_V1 = 0x20
} psyq_gpu_control_code_e;

typedef enum {
    PSYQ_GPU_INFO_DRAW_AREA_TOP_LEFT = 3,
    PSYQ_GPU_INFO_DRAW_AREA_BOTTOM_RIGHT = 4,
    PSYQ_GPU_INFO_DRAW_OFFSET = 5,
    PSYQ_GPU_INFO_VERSION = 7
} psyq_gpu_info_e;

#define PSYQ_GPU_PARAMETER_MASK    0x00ffffffU
#define PSYQ_GPU_RGB_MASK          0x00ffffffU
#define PSYQ_GPU_MASK_SET          0x00000001U
#define PSYQ_GPU_MASK_TEST         0x00000002U
#define PSYQ_GPU_DISPLAY_DISABLED  0x00000001U
#define PSYQ_GPU_VRAM_2MB          0x00000001U
#define PSYQ_GPU_DMA_REQUEST_OFF   0x00000000U
#define PSYQ_GPU_DMA_REQUEST_WRITE 0x00000002U
#define PSYQ_GPU_DMA_REQUEST_READ  0x00000003U

/* GPU status when GP1 is read, rather than a command written to GP1. */
#define PSYQ_GPU_STATUS_HEIGHT_480      0x00080000U
#define PSYQ_GPU_STATUS_READY_COMMAND   0x04000000U
#define PSYQ_GPU_STATUS_READ_DATA_READY 0x08000000U
#define PSYQ_GPU_STATUS_READY_DMA       0x10000000U
#define PSYQ_GPU_STATUS_ODD_LINE        0x80000000U

/* CPU DMA channel control and priority registers. */
#define PSYQ_GPU_DMA_TO_DEVICE         0x00000001U
#define PSYQ_GPU_DMA_ADDRESS_DECREMENT 0x00000002U
#define PSYQ_GPU_DMA_MODE_REQUEST      0x00000200U
#define PSYQ_GPU_DMA_MODE_LINKED_LIST  0x00000400U
#define PSYQ_GPU_DMA_BUSY              0x01000000U
#define PSYQ_GPU_DMA_TRIGGER           0x10000000U
#define PSYQ_GPU_DMA_GPU_ENABLE        0x00000800U
#define PSYQ_GPU_DMA_OTC_ENABLE        0x08000000U
#define PSYQ_GPU_DMA_ADDRESS_MASK      0x00ffffffU
#define PSYQ_GPU_DMA_END               0x00ffffffU
#define PSYQ_GPU_DMA_IMAGE_BLOCK_WORDS 16U
#define PSYQ_GPU_DMA_GPU_CHANNEL       2
#define PSYQ_GPU_DMA_OTC_CHANNEL       6

/* GP1 display-mode payload. */
#define PSYQ_GPU_DISPLAY_WIDTH_320  0x01U
#define PSYQ_GPU_DISPLAY_WIDTH_512  0x02U
#define PSYQ_GPU_DISPLAY_WIDTH_640  0x03U
#define PSYQ_GPU_DISPLAY_HEIGHT_480 0x04U
#define PSYQ_GPU_DISPLAY_PAL        0x08U
#define PSYQ_GPU_DISPLAY_RGB24      0x10U
#define PSYQ_GPU_DISPLAY_INTERLACE  0x20U
#define PSYQ_GPU_DISPLAY_WIDTH_368  0x40U
#define PSYQ_GPU_DISPLAY_REVERSE    0x80U

/* GPU packet header: low 24 bits link to the next primitive; high 8 bits
 * contain the packet length. The SDK view continues with the first command
 * word (colour and GPU code), so it is 8 bytes; game code only uses it
 * through casts of primitive pointers. */
typedef struct {
    u32 addr : 24;
    u32 len : 8;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
} P_TAG;

#define setaddr(p, a)  (((P_TAG*)(p))->addr = (u32)(a))
#define getaddr(p)     ((u32)((P_TAG*)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} RECT;

typedef struct {
    u32 tag;
    u32 code[15];
} DR_ENV;

typedef struct {
    RECT clip;
    s16 ofs[2];
    RECT tw;
    u16 tpage;
    u8 dtd;
    u8 dfe;
    u8 isbg;
    u8 r0;
    u8 g0;
    u8 b0;
    DR_ENV dr_env;
} DRAWENV;

typedef struct {
    RECT disp;
    RECT screen;
    u8 isinter;
    u8 isrgb24;
    u8 pad0;
    u8 pad1;
} DISPENV;

typedef struct {
    u32 mode;
    RECT* crect;
    u32* caddr;
    RECT* prect;
    u32* paddr;
} TIM_IMAGE;

typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
} POLY_F4;

typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
} POLY_F3;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
} POLY_G3;

/* Untextured Gouraud quad. */
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
    u8 r3, g3, b3, pad3;
    s16 x3, y3;
} POLY_G4;
typedef char assert_poly_g4_size[sizeof(POLY_G4) == 36 ? 1 : -1];

typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;
    s16 x1;
    s16 y1;
    u8 u1;
    u8 v1;
    u16 tpage;
    s16 x2;
    s16 y2;
    u8 u2;
    u8 v2;
    u16 pad1;
    s16 x3;
    s16 y3;
    u8 u3;
    u8 v3;
    u16 pad2;
} POLY_FT4;

/* Flat-shaded textured triangle packet. */
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad1;
} POLY_FT3;

typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 p1;
    s16 x1;
    s16 y1;
    u8 u1;
    u8 v1;
    u16 tpage;
    u8 r2;
    u8 g2;
    u8 b2;
    u8 p2;
    s16 x2;
    s16 y2;
    u8 u2;
    u8 v2;
    u16 pad2;
} POLY_GT3;

typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 p1;
    s16 x1;
    s16 y1;
    u8 u1;
    u8 v1;
    u16 tpage;
    u8 r2;
    u8 g2;
    u8 b2;
    u8 p2;
    s16 x2;
    s16 y2;
    u8 u2;
    u8 v2;
    u16 pad2;
    u8 r3;
    u8 g3;
    u8 b3;
    u8 p3;
    s16 x3;
    s16 y3;
    u8 u3;
    u8 v3;
    u16 pad3;
} POLY_GT4;

typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;
    s16 w;
    s16 h;
} SPRT;

typedef struct {
    u32 tag;
    u32 code[2];
} DR_MODE;

typedef struct {
    u32 tag;
    u32 code[2];
} DR_PRIO;

/* libgpu DR_MOVE (u32 code[5]); the VRAM-to-VRAM move command words are
 * spelled out so game code can fill the source/destination halfwords. */
typedef struct {
    u32 tag;
    u32 code[2]; /* 0x04: cache flush, move command */
    s16 x0;      /* 0x0c: source */
    s16 y0;
    s16 x1; /* 0x10: destination */
    s16 y1;
    s16 w; /* 0x14 */
    s16 h;
} DR_MOVE;

typedef struct {
    u32 tag;
    u32 code[2];
} DR_AREA;

typedef struct {
    u32 tag;
    u32 code[2];
} DR_OFFSET;

typedef struct {
    u32 tag;
    u32 code[1];
} DR_TPAGE;

/* Gouraud line. */
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
} LINE_G2;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
} LINE_F2;

typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    s16 w;
    s16 h;
} TILE;

typedef char assert_rect_size[sizeof(RECT) == 8 ? 1 : -1];
typedef char assert_dr_env_size[sizeof(DR_ENV) == 64 ? 1 : -1];
typedef char assert_drawenv_size[sizeof(DRAWENV) == 92 ? 1 : -1];
typedef char assert_dispenv_size[sizeof(DISPENV) == 20 ? 1 : -1];
typedef char assert_tim_image_size[sizeof(TIM_IMAGE) == 20 ? 1 : -1];
typedef char assert_poly_f3_size[sizeof(POLY_F3) == 20 ? 1 : -1];
typedef char assert_poly_g3_size[sizeof(POLY_G3) == 28 ? 1 : -1];
typedef char assert_poly_f4_size[sizeof(POLY_F4) == 24 ? 1 : -1];
typedef char assert_poly_ft4_size[sizeof(POLY_FT4) == 40 ? 1 : -1];
typedef char assert_poly_gt3_size[sizeof(POLY_GT3) == 40 ? 1 : -1];
typedef char assert_poly_gt4_size[sizeof(POLY_GT4) == 52 ? 1 : -1];
typedef char assert_sprt_size[sizeof(SPRT) == 20 ? 1 : -1];
typedef char assert_dr_mode_size[sizeof(DR_MODE) == 12 ? 1 : -1];
typedef char assert_dr_offset_size[sizeof(DR_OFFSET) == 12 ? 1 : -1];
typedef char assert_dr_tpage_size[sizeof(DR_TPAGE) == 8 ? 1 : -1];
typedef char assert_p_tag_size[sizeof(P_TAG) == 8 ? 1 : -1];
typedef char assert_tile_size[sizeof(TILE) == 16 ? 1 : -1];
typedef char assert_line_g2_size[sizeof(LINE_G2) == 20 ? 1 : -1];
typedef char assert_line_f2_size[sizeof(LINE_F2) == 16 ? 1 : -1];

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0, x1, y1, x2, y2;
    u32 pad;
} LINE_F3;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
    u8 r2, g2, b2, p2;
    s16 x2, y2;
    u32 pad;
} LINE_G3;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0, x1, y1, x2, y2, x3, y3;
    u32 pad;
} LINE_F4;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
    u8 r2, g2, b2, p2;
    s16 x2, y2;
    u8 r3, g3, b3, p3;
    s16 x3, y3;
    u32 pad;
} LINE_G4;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
} SPRT_8;

/* Resolved retail TMD record used by ReadTMD; field offsets are measured. */
typedef struct {
    u32 id;
    CVECTOR color0, color1, color2, color3;
    u16 tpage, clut;
    u8 u0, v0, u1, v1, u2, v2, u3, v3;
    SVECTOR x0, x1, x2, x3;
    SVECTOR n0, n1, n2, n3;
    SVECTOR* vertex_base;
    SVECTOR* normal_base;
    u16 vertex0, vertex1, vertex2, vertex3;
    u16 normal0, normal1, normal2, normal3;
} psyq_tmd_primitive_t;

extern void FntLoad(int, int);
extern int FntOpen(int, int, int, int, int, int);
extern int FntPrint(char*, ...);
extern u32* FntFlush(int);
extern void SetDumpFnt(int);
extern int OpenTIM(void*);
extern int ReadTIM(TIM_IMAGE*);
extern u16 LoadTPage(u32*, int, int, int, int, int, int);
extern u16 LoadClut2(u32*, int, int);
extern void AddPrim(void*, void*);
void AddPrims(u32* ot, void* first, void* last);
extern void SetSemiTrans(void*, int);
extern void SetShadeTex(void*, int);
extern void SetPolyF3(POLY_F3*);
extern void SetPolyFT3(void*);
extern void SetPolyG3(void*);
extern void SetPolyGT3(void*);
extern void SetPolyF4(POLY_F4*);
extern void SetPolyFT4(POLY_FT4*);
extern void SetPolyG4(void*);
extern void SetPolyGT4(void*);
extern void SetSprt8(void*);
extern void SetSprt16(void*);
extern void SetSprt(void*);
extern void SetTile1(void*);
extern void SetTile8(void*);
extern void SetTile16(void*);
extern void SetTile(void*);
extern void SetLineF2(void*);
extern void SetLineG2(void*);
extern void SetLineF3(void*);
extern void SetLineG3(void*);
extern void SetLineF4(void*);
extern void SetLineG4(void*);
extern void SetBlockFill(void*);
extern void SetDrawMove(void*);
extern u16 GetTPage(int tp, int abr, int x, int y);
extern u16 GetClut(int x, int y);
extern DR_MODE* SetDrawMode(DR_MODE*, int, int, int, RECT*);
extern DR_AREA* SetDrawArea(DR_AREA*, RECT*);
extern void SetDispMask(int);
extern int ResetGraph(int);
extern int SetGraphDebug(int);
extern int GetGraphType(void);
extern void* DrawSyncCallback(void*);
extern int DrawSync(int);
extern void ClearImage(RECT*, u8, u8, u8);
extern u32* ClearOTag(void*, int);
extern u32* ClearOTagR(u32*, int);
extern s32 LoadImage(RECT*, u32*);
extern s32 StoreImage(RECT*, u32*);
extern s32 MoveImage(RECT*, s32, s32);
extern void DrawOTag(u32* ot);
extern DRAWENV* SetDefDrawEnv(DRAWENV*, int, int, int, int);
extern DISPENV* SetDefDispEnv(DISPENV*, int, int, int, int);
extern DRAWENV* GetDrawEnv(DRAWENV*);
extern DRAWENV* PutDrawEnv(DRAWENV*);
extern DISPENV* PutDispEnv(DISPENV*);

u16 LoadClut(u32* clut, s32 x, s32 y);
void SetDrawOffset(void* destination, void* position);

void DrawPrim(void* p);

void DumpTPage(u16 tpage);
void DumpClut(u16 clut);
int GetGraphDebug(void);
int SetGraphReverse(int reversed);
int SetGraphQueue(int enabled);
void DumpDrawEnv(DRAWENV* env);
void DumpDispEnv(DISPENV* env);
void SetDrawEnv(DR_ENV* packet, DRAWENV* env);
void DrawOTagEnv(u32* ot, DRAWENV* env);
DISPENV* GetDispEnv(DISPENV* env);
u32 GetODE(void);
void SetTexWindow(void* packet, RECT* window);
void SetPriority(DR_PRIO* packet, int test_mask, int set_mask);
void* NextPrim(void* primitive);
int IsEndPrim(void* primitive);
void CatPrim(void* primitive, void* next);
void TermPrim(void* primitive);
void SetDrawTPage(void* primitive, int dfe, int dtd, int tpage);
void SetDrawLoad(void* primitive, RECT* rect);
int MargePrim(void* first, void* second);
int OpenTMD(u32* tmd, int object_index);
psyq_tmd_primitive_t* ReadTMD(psyq_tmd_primitive_t* primitive);

/* driver internals */

typedef union {
    RECT rect;
    u32 words[2];
} psyq_gpu_rect_words_t;

typedef struct {
    u32 tag;
    u32 cache_flush;
    u32 command;
    psyq_gpu_rect_words_t rect;
} psyq_draw_load_t;

typedef int (*psyq_gpu_operation_t)(void*, u32);
typedef struct {
    const char* name;
    int (*enqueue_three)(psyq_gpu_operation_t, void*, u32);
    int (*enqueue_four)(psyq_gpu_operation_t, void*, int, u32);
    psyq_gpu_operation_t clear;
    void (*control)(u32);
    int (*commands)(u32*, int);
    psyq_gpu_operation_t ordering_table;
    psyq_gpu_operation_t store;
    psyq_gpu_operation_t load;
    int (*process_queue)(void);
    u32 (*cached_control)(int);
    int (*clear_ot_reverse)(u32*, int);
    u32 (*information)(int);
    int (*reset)(int);
    u32 (*status)(void);
    int (*sync)(int);
} psyq_gpu_dispatch_t;

typedef struct {
    u8 graph_type;
    u8 queue_enabled;
    u8 debug_level;
    u8 reversed;
    u16 vram_width;
    u16 vram_height;
    s32 completion_pending;
    void* draw_sync_callback;
    DRAWENV draw;
    DISPENV display;
} psyq_gpu_environment_t;

extern psyq_gpu_environment_t g_psyq_gpu_environment;

extern psyq_gpu_dispatch_t* g_psyq_gpu_dispatch;
extern psyq_gpu_dispatch_t g_psyq_gpu_dispatch_table;
extern int (*g_psyq_gpu_printf)(const char*, ...);
extern u8 g_psyq_gpu_graph_type;
extern u8 g_psyq_gpu_queue_enabled;
extern u8 g_psyq_gpu_debug_level;
extern u8 g_psyq_gpu_graph_reverse;
extern u16 g_psyq_gpu_vram_width;
extern u16 g_psyq_gpu_vram_height;
extern s32 g_psyq_gpu_completion_pending;
extern void* g_psyq_gpu_draw_sync_callback;
extern DRAWENV g_psyq_gpu_draw_environment;
extern DISPENV g_psyq_gpu_display_environment;
extern volatile u32* g_psyq_gpu_gp0_port;
extern volatile u32* g_psyq_gpu_gp1_port;
extern volatile u32* g_psyq_gpu_dma_madr;
extern volatile u32* g_psyq_gpu_dma_bcr;
extern volatile u32* g_psyq_gpu_dma_chcr;
extern volatile u32* g_psyq_gpu_otc_dma_madr;
extern volatile u32* g_psyq_gpu_otc_dma_bcr;
extern volatile u32* g_psyq_gpu_otc_dma_chcr;
extern volatile u32* g_psyq_gpu_dma_dpcr;
extern u8 g_psyq_gpu_gp1_cache[256];
extern u32* g_psyq_gpu_tim_cursor;
extern int g_psyq_gpu_font_window_count;
extern int g_psyq_gpu_default_font_window;
extern u8 g_psyq_gpu_font_asset[];

typedef struct {
    TILE tile;
    DR_MODE mode;
    int character_limit;
    SPRT_8* sprites;
    char* text;
    int text_count;
    int wrap_disabled;
} psyq_font_window_t;

extern psyq_font_window_t g_psyq_gpu_font_windows[8];
extern int g_psyq_gpu_font_allocated_characters;
extern const char* g_psyq_gpu_font_hex_digits;
extern char g_psyq_gpu_font_text_pool[1024];
extern SPRT_8 g_psyq_gpu_font_sprite_pool[1024];
extern u16 g_psyq_gpu_font_tpage;
extern u16 g_psyq_gpu_font_clut;
extern int g_psyq_gpu_sync_deadline;
extern int g_psyq_gpu_sync_poll_count;
extern const u16 g_psyq_gpu_vram_width_lookup[10];
extern const u16 g_psyq_gpu_vram_height_lookup[10];
extern int g_psyq_gpu_queue_head;
extern volatile int g_psyq_gpu_queue_tail;
extern int g_psyq_gpu_reset_interrupt_mask;
typedef struct {
    psyq_gpu_operation_t volatile operation;
    void* volatile source;
    volatile u32 argument;
    u32 packet_words[21];
} psyq_gpu_queue_entry_t;

/* A packet member repeats every 96 bytes; its tail overlaps the next entry's metadata. */
typedef struct {
    u32 packet_words[21];
    u32 _unknown_54[3];
} psyq_gpu_queue_packet_view_t;

typedef struct {
    psyq_gpu_operation_t operation;
    void* source;
    u32 argument;
} psyq_gpu_operation_snapshot_t;

extern psyq_gpu_queue_entry_t g_psyq_gpu_operation_queue[64];
extern int g_psyq_gpu_enqueue_interrupt_mask;
extern int g_psyq_gpu_queue_interrupt_mask;

u32 _status(void);
void _ctl(u32 command);
u32 _getctl(int command);
int _cwb(u32* commands, int count);
void _cwc(u32* ot);
u32 _param(int command);
void memset2(void* destination, int value, int size);
int psyq_gpu_decode_tim(u32* tim, TIM_IMAGE* image);
int _addque(psyq_gpu_operation_t operation, void* source, u32 argument);
int _addque2(psyq_gpu_operation_t operation, void* source, int copy_size, u32 argument);
void set_alarm(void);
int _reset(int mode);
int psyq_gpu_detect_type(int mode);

extern const char g_psyq_gpu_tim_id_format[];
extern const char g_psyq_gpu_tim_mode_format[];
extern const char g_psyq_gpu_tim_address_format[];
extern const char g_psyq_gpu_rectangle_operation_format[];
extern const char g_psyq_gpu_clear_image_name[];
extern const char g_psyq_gpu_bad_rectangle_format[];
extern const char g_psyq_gpu_rectangle_format[];
extern const char g_psyq_gpu_load_image_name[];
extern const char g_psyq_gpu_store_image_name[];
extern const char g_psyq_gpu_move_image_name[];
extern const char g_psyq_gpu_draw_otag_format[];
extern const char g_psyq_gpu_draw_sync_format[];
extern const char g_psyq_gpu_draw_sync_callback_format[];
extern const char g_psyq_gpu_graph_debug_format[];
extern const char g_psyq_gpu_tpage_format[];
extern const char g_psyq_gpu_clut_format[];
extern const char g_psyq_gpu_draw_clip_format[];
extern const char g_psyq_gpu_draw_offset_format[];
extern const char g_psyq_gpu_texture_window_format[];
extern const char g_psyq_gpu_dithering_format[];
extern const char g_psyq_gpu_draw_display_enable_format[];
extern const char g_psyq_gpu_display_rectangle_format[];
extern const char g_psyq_gpu_screen_rectangle_format[];
extern const char g_psyq_gpu_interlace_format[];
extern const char g_psyq_gpu_rgb24_format[];
extern const char g_psyq_gpu_display_mask_format[];
extern const char g_psyq_gpu_clear_otag_format[];
extern const char g_psyq_gpu_clear_otag_reverse_format[];
extern const char g_psyq_gpu_graph_reverse_format[];
extern const char g_psyq_gpu_graph_queue_format[];
extern const char g_psyq_gpu_reset_addresses_format[];
extern const char g_psyq_gpu_reset_graph_format[];
extern const char g_psyq_gpu_put_draw_env_format[];
extern const char g_psyq_gpu_draw_otag_env_format[];
extern const char g_psyq_gpu_put_disp_env_format[];
extern u16 g_psyq_gpu_cached_display_x, g_psyq_gpu_cached_display_y, g_psyq_gpu_cached_display_width,
    g_psyq_gpu_cached_display_height;
extern u16 g_psyq_gpu_cached_screen_x, g_psyq_gpu_cached_screen_y, g_psyq_gpu_cached_screen_width,
    g_psyq_gpu_cached_screen_height;
extern u32 g_psyq_gpu_cached_display_flags;
extern u32 g_psyq_gpu_ot_terminator;

typedef struct {
    u32 tag;
    u32 command;
    u32 source;
    u32 destination;
    u32 dimensions;
} psyq_gpu_move_packet_t;

extern psyq_gpu_move_packet_t g_psyq_gpu_move_packet;
extern u32 g_psyq_gpu_move_copy_words[3];
extern u32 g_psyq_gpu_move_destination;
extern u32 g_psyq_gpu_move_dimensions;

u32 get_mode(int dfe, int dtd, int tpage);
u32 get_cs(int x, int y);
u32 get_ce(int x, int y);
u32 get_ofs(int x, int y);
u32 get_tw(RECT* rect);
int get_dx(RECT* rect);
int get_alarm(void);
int _otc(u32* ot, int count);
void checkRECT(const char* name, RECT* rect);

typedef struct {
    u32 id, flags, object_count;
} psyq_tmd_header_t;

typedef struct {
    u32 vertex_offset, vertex_count;
    u32 normal_offset, normal_count;
    u32 primitive_offset, primitive_count;
    u32 scale;
} psyq_tmd_object_t;

extern SVECTOR* g_psyq_gpu_tmd_vertices;
extern SVECTOR* g_psyq_gpu_tmd_normals;
extern u8* g_psyq_gpu_tmd_cursor;
extern int g_psyq_gpu_tmd_primitive_count;
extern const char g_psyq_gpu_tmd_decode_message[];
extern const char g_psyq_gpu_tmd_object_format[];
extern const char g_psyq_gpu_tmd_vertices_format[];
extern const char g_psyq_gpu_tmd_normals_format[];
extern const char g_psyq_gpu_tmd_primitives_format[];
int psyq_gpu_open_tmd_object(
    psyq_tmd_header_t* header, int object_index, u8** primitive, SVECTOR** vertices, SVECTOR** normals);
int psyq_gpu_decode_tmd_primitive(const u8* source, psyq_tmd_primitive_t* primitive);

extern psyq_gpu_operation_t g_psyq_gpu_last_operation;
extern void* g_psyq_gpu_last_source;
extern u32 g_psyq_gpu_last_argument;
extern const char g_psyq_gpu_timeout_state_format[];
extern const char g_psyq_gpu_timeout_operation_format[];
int _exeque(void);
int _sync(int mode);

/* Tile and block-fill paths reuse these words; separate aliases preserve the original address loads. */
extern u32 g_psyq_gpu_clear_packet_tag;
extern u32 g_psyq_gpu_clear_packet_word_1;
extern u32 g_psyq_gpu_clear_packet_word_2;
extern u32 g_psyq_gpu_clear_packet_word_3;
extern u32 g_psyq_gpu_clear_packet_word_4;
extern u32 g_psyq_gpu_clear_packet_word_5;
extern u32 g_psyq_gpu_clear_packet_word_6;
extern u32 g_psyq_gpu_clear_packet_word_7;
extern u32 g_psyq_gpu_clear_restore_tag;
extern u32 g_psyq_gpu_clear_restore_area_start;
extern u32 g_psyq_gpu_clear_restore_area_end;
extern u32 g_psyq_gpu_clear_restore_offset;
int _clr(void* rectangle, u32 color);
int _dws(void* rectangle, u32 pixels);
int _drs(void* rectangle, u32 pixels);

/* Input layouts used by the retail TMD converter; lengths include its header. */
typedef enum {
    PSYQ_TMD_PACKET_F3_LIT = 0x20000304,
    PSYQ_TMD_PACKET_G3_LIT = 0x30000406,
    PSYQ_TMD_PACKET_FT3_LIT = 0x24000507,
    PSYQ_TMD_PACKET_GT3_LIT = 0x34000609,
    PSYQ_TMD_PACKET_F3 = 0x21010304,
    PSYQ_TMD_PACKET_G3 = 0x31010506,
    PSYQ_TMD_PACKET_FT3 = 0x25010607,
    PSYQ_TMD_PACKET_GT3 = 0x35010809,
    PSYQ_TMD_PACKET_F4_LIT = 0x28000405,
    PSYQ_TMD_PACKET_G4_LIT = 0x38000508,
    PSYQ_TMD_PACKET_FT4_LIT = 0x2c000709,
    PSYQ_TMD_PACKET_GT4_LIT = 0x3c00080c,
    PSYQ_TMD_PACKET_F4 = 0x29010305,
    PSYQ_TMD_PACKET_G4 = 0x39010608,
    PSYQ_TMD_PACKET_FT4 = 0x2d010709,
    PSYQ_TMD_PACKET_GT4 = 0x3d010a0c,
    PSYQ_TMD_PACKET_MODE_MASK = 0xfdffffffU,
} psyq_tmd_packet_id_e;

typedef struct {
    u32 id;
    CVECTOR color0;
    u16 normal0;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
} psyq_tmd_f3_lit_packet_t;

typedef struct {
    u32 id;
    CVECTOR color0;
    u16 normal0;
    u16 vertex0;
    u16 normal1;
    u16 vertex1;
    u16 normal2;
    u16 vertex2;
} psyq_tmd_g3_lit_packet_t;

typedef struct {
    u32 id;
    u8 u0;
    u8 v0;
    u16 clut;
    u8 u1;
    u8 v1;
    u16 tpage;
    u8 u2;
    u8 v2;
    u8 _unknown_0e[2];
    u16 normal0;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
} psyq_tmd_ft3_lit_packet_t;

typedef struct {
    u32 id;
    u8 u0;
    u8 v0;
    u16 clut;
    u8 u1;
    u8 v1;
    u16 tpage;
    u8 u2;
    u8 v2;
    u8 _unknown_0e[2];
    u16 normal0;
    u16 vertex0;
    u16 normal1;
    u16 vertex1;
    u16 normal2;
    u16 vertex2;
} psyq_tmd_gt3_lit_packet_t;

typedef struct {
    u32 id;
    CVECTOR color0;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u8 _unknown_0e[2];
} psyq_tmd_f3_packet_t;

typedef struct {
    u32 id;
    CVECTOR color0;
    CVECTOR color1;
    CVECTOR color2;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u8 _unknown_16[2];
} psyq_tmd_g3_packet_t;

typedef struct {
    u32 id;
    u8 u0;
    u8 v0;
    u16 clut;
    u8 u1;
    u8 v1;
    u16 tpage;
    u8 u2;
    u8 v2;
    u8 _unknown_0e[2];
    CVECTOR color0;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u8 _unknown_1a[2];
} psyq_tmd_ft3_packet_t;

typedef struct {
    u32 id;
    u8 u0;
    u8 v0;
    u16 clut;
    u8 u1;
    u8 v1;
    u16 tpage;
    u8 u2;
    u8 v2;
    u8 _unknown_0e[2];
    CVECTOR color0;
    CVECTOR color1;
    CVECTOR color2;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u8 _unknown_22[2];
} psyq_tmd_gt3_packet_t;

typedef struct {
    u32 id;
    CVECTOR color0;
    u16 normal0;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u16 vertex3;
    u8 _unknown_12[2];
} psyq_tmd_f4_lit_packet_t;

typedef struct {
    u32 id;
    CVECTOR color0;
    u16 normal0;
    u16 vertex0;
    u16 normal1;
    u16 vertex1;
    u16 normal2;
    u16 vertex2;
    u16 normal3;
    u16 vertex3;
} psyq_tmd_g4_lit_packet_t;

typedef struct {
    u32 id;
    u8 u0;
    u8 v0;
    u16 clut;
    u8 u1;
    u8 v1;
    u16 tpage;
    u8 u2;
    u8 v2;
    u8 _unknown_0e[2];
    u8 u3;
    u8 v3;
    u8 _unknown_12[2];
    u16 normal0;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u16 vertex3;
    u8 _unknown_1e[2];
} psyq_tmd_ft4_lit_packet_t;

typedef struct {
    u32 id;
    u8 u0;
    u8 v0;
    u16 clut;
    u8 u1;
    u8 v1;
    u16 tpage;
    u8 u2;
    u8 v2;
    u8 _unknown_0e[2];
    u8 u3;
    u8 v3;
    u8 _unknown_12[2];
    u16 normal0;
    u16 vertex0;
    u16 normal1;
    u16 vertex1;
    u16 normal2;
    u16 vertex2;
    u16 normal3;
    u16 vertex3;
} psyq_tmd_gt4_lit_packet_t;

typedef struct {
    u32 id;
    CVECTOR color0;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u16 vertex3;
} psyq_tmd_f4_packet_t;

typedef struct {
    u32 id;
    CVECTOR color0;
    CVECTOR color1;
    CVECTOR color2;
    CVECTOR color3;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u16 vertex3;
} psyq_tmd_g4_packet_t;

typedef struct {
    u32 id;
    u8 u0;
    u8 v0;
    u16 clut;
    u8 u1;
    u8 v1;
    u16 tpage;
    u8 u2;
    u8 v2;
    u8 _unknown_0e[2];
    u8 u3;
    u8 v3;
    u8 _unknown_12[2];
    CVECTOR color0;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u16 vertex3;
} psyq_tmd_ft4_packet_t;

typedef struct {
    u32 id;
    u8 u0;
    u8 v0;
    u16 clut;
    u8 u1;
    u8 v1;
    u16 tpage;
    u8 u2;
    u8 v2;
    u8 _unknown_0e[2];
    u8 u3;
    u8 v3;
    u8 _unknown_12[2];
    CVECTOR color0;
    CVECTOR color1;
    CVECTOR color2;
    CVECTOR color3;
    u16 vertex0;
    u16 vertex1;
    u16 vertex2;
    u16 vertex3;
} psyq_tmd_gt4_packet_t;

typedef union {
    u32 id;
    psyq_tmd_f3_lit_packet_t f3_lit;
    psyq_tmd_g3_lit_packet_t g3_lit;
    psyq_tmd_ft3_lit_packet_t ft3_lit;
    psyq_tmd_gt3_lit_packet_t gt3_lit;
    psyq_tmd_f3_packet_t f3;
    psyq_tmd_g3_packet_t g3;
    psyq_tmd_ft3_packet_t ft3;
    psyq_tmd_gt3_packet_t gt3;
    psyq_tmd_f4_lit_packet_t f4_lit;
    psyq_tmd_g4_lit_packet_t g4_lit;
    psyq_tmd_ft4_lit_packet_t ft4_lit;
    psyq_tmd_gt4_lit_packet_t gt4_lit;
    psyq_tmd_f4_packet_t f4;
    psyq_tmd_g4_packet_t g4;
    psyq_tmd_ft4_packet_t ft4;
    psyq_tmd_gt4_packet_t gt4;
} psyq_tmd_packet_t;

extern const char g_psyq_gpu_tmd_f3_lit_name[];
extern const char g_psyq_gpu_tmd_g3_lit_name[];
extern const char g_psyq_gpu_tmd_ft3_lit_name[];
extern const char g_psyq_gpu_tmd_gt3_lit_name[];
extern const char g_psyq_gpu_tmd_f3_name[];
extern const char g_psyq_gpu_tmd_g3_name[];
extern const char g_psyq_gpu_tmd_ft3_name[];
extern const char g_psyq_gpu_tmd_gt3_name[];
extern const char g_psyq_gpu_tmd_f4_lit_name[];
extern const char g_psyq_gpu_tmd_g4_lit_name[];
extern const char g_psyq_gpu_tmd_ft4_lit_name[];
extern const char g_psyq_gpu_tmd_gt4_lit_name[];
extern const char g_psyq_gpu_tmd_f4_name[];
extern const char g_psyq_gpu_tmd_g4_name[];
extern const char g_psyq_gpu_tmd_ft4_name[];
extern const char g_psyq_gpu_tmd_gt4_name[];
extern const char g_psyq_gpu_unsupported_tmd_format[];

#endif
