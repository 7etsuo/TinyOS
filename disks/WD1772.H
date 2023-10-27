/* 
 * @file wd1772.h
 * @brief Macros for WD1772 Disk Controller Registers and Flags.
 */
#ifndef WD_H
#define WD_H

/**
 * ===========#==#=======#===============================================#=====
 * ----------------------|DMA,WD1772 Disk Controller                     |-----
 * ===========#==#=======#===============================================#=====
 * $FFFF8604.W|RW|WDC_SEC|FDC/HDC Access/Sector Count                    |
 * $FFFF8606.W|R-|WDC_DMA|DMA Status                  %________ _____DSE |
 *            |  |       | Data request condition--------------------+|| |
 *            |  |       | Sector count 0:zero,1:not zero-------------+| |
 *            |  |       | DMA 0:no error,1:error----------------------+ |
 *            |-W|       |DMA Mode                    %_______W FD_SHAA_ |
 *            |  |       | 0:read,1:write---------------------+ || ||||  |
 *            |  |       | 0:HDC,1:FDC access-------------------+| ||||  |
 *            |  |       | DMA 0:on,1:off------------------------+ ||||  |
 *            |  |       | 0:FDC/HDC register 1:sector count-------+|||  |
 *            |  |       | 0:FDC,1:HDC access-----------------------+||  |
 *            |  |       | A1/A0 pin 0:low,1:high--------------------++  |
 * $FFFF8609.B|RW|WDC_BSH|DMA Base and Counter Hi                        |
 * $FFFF860B.B|RW|WDC_BSM|DMA Base and Counter Mi                        |
 * $FFFF860D.B|RW|WDC_BSL|DMA Base and Counter Lo                        |
 * $FFFF860E.B|RW|WDC_BSL|Frequency and Density control        %______FD |STE,F
 *            |  |       |Frequency 0:8MHz,1:16MHz--------------------+| |
 *            |  |       |Density 0:DD,1:HD----------------------------+ |
 * ===========#==#=======#===============================================#=====
 */


/* @defgroup WD1772_Register_Definitions WD1772 Disk Controller Registers
 * @{
 */

/* Base address of the WD1772 Disk Controller */
#define WD1772_BASE_ADDR 	0xFFFF8600

/* FDC/HDC Access/Sector Count Register */
#define WDC_SEC_REG_ADDR	(WD1772_BASE_ADDR + 0x04)

/* DMA Status Register */
#define WDC_DMA_STATUS_ADDR	(WD1772_BASE_ADDR + 0x06)
/* DMA Mode Register */
#define WDC_DMA_MODE_ADDR	(WD1772_BASE_ADDR + 0x06)

/* DMA Base and Counter High Byte Register */
#define WDC_BSH_REG_ADDR	(WD1772_BASE_ADDR + 0x09)
/* DMA Base and Counter Middle Byte Register */
#define WDC_BSM_REG_ADDR	(WD1772_BASE_ADDR + 0x0B)
/* DMA Base and Counter Low Byte Register */
#define WDC_BSL_REG_ADDR	(WD1772_BASE_ADDR + 0x0D)

/* @} */  /* end of WD1772_Register_Definitions */

/* DMA Status Flags (Read)
 * $FFFF8606.W|R-|WDC_DMA|DMA Status                  %________ _____DSE |
 *            |  |       | Data request condition--------------------+|| |
 *            |  |       | Sector count 0:zero,1:not zero-------------+| |
 *            |  |       | DMA 0:no error,1:error----------------------+ |
 */

/* DMA Mode Flags (Write)
 *            |-W|       |DMA Mode                    %_______W FD_SHAA_ |
 *            |  |       | 0:read,1:write---------------------+ || ||||  |
 *            |  |       | 0:HDC,1:FDC access-------------------+| ||||  |
 *            |  |       | DMA 0:on,1:off------------------------+ ||||  |
 *            |  |       | 0:FDC/HDC register 1:sector count-------+|||  |
 *            |  |       | 0:FDC,1:HDC access-----------------------+||  |
 *            |  |       | A1/A0 pin 0:low,1:high--------------------++  |
 */

#endif // WD_H
