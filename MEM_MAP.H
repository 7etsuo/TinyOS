#ifndef MEM_MAP_H
#define MEM_MAP_H

/** 
 * NOTE: kernel data/stack are at addresses 0x000140 <= ... < 0x000800
 * since we're only using interrupt vectors 0-79
 * 
 * 600 - 7FF	kernel stack	( 512 bytes)
 * 140 - 5FF	kernel data		(1216 bytes)	<-- idea: 140-1FF for misc, 200-3FF for process table, 400-4FF for console driver, 500-5FF for misc
 * 000 - 13F	vector table
 */

/* 000 - 13F vector table */
#define VECTOR_TABLE_ADDR        0x000000 

/* 140-5FF kernel data (1216 bytes) */
/* 140 - 1FF misc */
#define CONSOLE_X_P_ADDR         0x000140
#define CONSOLE_Y_P_ADDR         0x000141
#define VBL_COUNTER_ADDR         0x000142
#define CURSOR_VISIBLE_ADDR      0x000144
/* 200 - 3FF process table */
#define CURR_PROC_ADDR           0x000200
#define RESCHED_NEEDED_ADDR      0x000202 /* 0=no, 1=yes, 2=yes with eventual trap restart (blocking) */
#define PROC_ADDR                0x000204 /* array of max_num_proc (4) process structures */
/* 400-4FF console driver */
#define KYBD_ISR_STATE_ADDR      0x000400 /* 0=not in mouse packet, 1=expecting delta x, 2=expecting delta y */
#define KYBD_BUFF_HEAD_ADDR      0x000402
#define KYBD_BUFF_TAIL_ADDR      0x000404
#define KYBD_BUFF_FILL_ADDR      0x000406
#define KYBD_NUM_LINES_ADDR      0x000408
#define KYBD_LEN_LINE_ADDR       0x00040A /* number of characters in buffer for current line */
#define KYBD_SHIFTED_ADDR        0x00040C
#define KYBD_AUTO_CH_ADDR        0x00040E
#define KYBD_AUTO_COUNT_ADDR     0x000410
#define KYBD_BLOCKED_PROC_ADDR   0x000412
#define KYBD_FG_PROC_ADDR        0x000414
#define KYBD_BUFF_ADDR           0x000416 /* 128 byte circular queue - must be a power of 2 */
/* 500-5FF for misc (currently unused) */
/* end kernel data */
/* 600-7FF kernel stack (512 bytes) */

#define VIDEO_BASE          0x3F8000 /* frame buffer address */

#define MEM_CONFIG_REG_ADDR 0xFF8001

#define VIDEO_BASE_REG_ADDR 0xFF8201
#define VIDEO_BASE_HI_ADDR  VIDEO_BASE_REG_ADDR
#define VIDEO_BASE_MID_ADDR 0xFF8203
#define VIDEO_SYNC_REG_ADDR 0xFF820A
#define VIDEO_PAL0_REG_ADDR 0xFF8240
#define VIDEO_REZ_REG_ADDR  0xFF8260

#define MFP_IERA_ADDR       0xFFFA07
#define MFP_IERB_ADDR       0xFFFA09
#define MFP_ISRA_ADDR       0xFFFA0F
#define MFP_ISRB_ADDR       0xFFFA11
#define MFP_IMRA_ADDR       0xFFFA13
#define MFP_IMRB_ADDR       0xFFFA15
#define MFP_VR_ADDR         0xFFFA17
#define MFP_TACR_ADDR       0xFFFA19
#define MFP_TADR_ADDR       0xFFFA1F

#define IKBD_ACIA_CR_ADDR   0xFFFC00
#define IKBD_ACIA_SR_ADDR   0xFFFC00
#define IKBD_ACIA_RDR_ADDR  0xFFFC02

#define MIDI_ACIA_CR_ADDR   0xFFFC04 

#endif /* MEM_MAP_H */
