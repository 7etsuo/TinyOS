#include "../types.h"
#include "../hal/mem_map.h"
#include "psg.h"

IO_PORT8 PSG_reg_select = (IO_PORT8)REG_SELECT_ADDR;
IO_PORT8 PSG_reg_write = (IO_PORT8)REG_WRITE_ADDR;

void write_psg(Register reg, UINT8 val) 
{
    *PSG_reg_select = reg;
    *PSG_reg_write = val;
}

UINT8 read_psg(Register reg)
{
    *PSG_reg_select = reg;
    return *PSG_reg_select;
}

void set_tone(Channel channel, UINT16 tuning) 
{
    write_psg(FINE_TONE_REGS[channel], tuning >> 4 & 0x0F);
    write_psg(ROUGH_TONE_REGS[channel], tuning & 0x000F);
}

void set_volume(Channel channel, UINT8 volume)
{
    write_psg(VOLUME_REGS[channel], volume);
}

void enable_channel(Channel channel, UINT8 tone_on, UINT8 noise_on)
{
    UINT8 index;

    index = 3 * channel + tone_on + 2 * noise_on;
    write_psg(R7, (!tone_on && !noise_on) ? IO_CONFIGS[OFF] : IO_CONFIGS[CONFIGS[index]]);
}

void stop_sound()
{
    UINT8 count;
    for (count = 1; count <= 0xD; ++count) {
        write_psg((Register)count, 0);
    }
}

void set_noise(UINT8 tuning)
{
    write_psg(R6, tuning);
}

void set_envelope(EnvShape shape, UINT16 sustain)
{
    write_psg(RB, sustain >> 8 & 0xFF);
    write_psg(RC, sustain & 0xFF);
    write_psg(RD, ENV_SHAPES[shape]);
}

void control_portA(UINT8 mask, UINT8 value)
{
    UINT8 current;
    current = read_psg(PORT_A);
    current = (current & mask) | value;
    write_psg(PORT_A, current);
}

void control_portB(UINT8 value) 
{
    write_psg(PORT_B, value);
}

