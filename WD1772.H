#ifndef WD_H
#define WD_H

// TODO: macro
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

#endif // WD_H
