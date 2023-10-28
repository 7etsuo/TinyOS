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
    static const Register FINE_TONE_REGS[] = { R0, R2, R4 };
    static const Register ROUGH_TONE_REGS[] = { R1, R3, R5 };
    write_psg(FINE_TONE_REGS[channel], tuning >> 4 & 0x0F);
    write_psg(ROUGH_TONE_REGS[channel], tuning & 0x000F);
}

void set_volme(Channel channel, UINT8 volume)
{
    static const Register VOLUME_REGS[] = { R8, R9, RA };
    write_psg(VOLUME_REGS[channel], volume);
}

void enable_channel(Channel channel, UINT8 tone_on, UINT8 noise_on)
{
    UINT8 index = 3 * channel + tone_on + 2 * noise_on;
    static const IOConfig CONFIGS[] = { A_NOISEON_TONEON, A_NOISEON_TONEOFF, A_NOISEOFF_TONEON, B_NOISEON_TONEON, B_NOISEON_TONEOFF, B_NOISEOFF_TONEON, C_NOISEON_TONEON, C_NOISEON_TONEOFF, C_NOISEOFF_TONEON };
    write_psg(R7, (!tone_on && !noise_on) ? IO_CONFIGS[OFF] : IO_CONFIGS[CONFIGS[index]]);
}

void stop_sound()
{
    for (UINT8 count = 1; count <= 0xD; ++count) {
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
    UINT8 current = read_psg(PORT_A);
    current = (current & mask) | value;
    write_psg(PORT_A, current);
}

void control_portB(UINT8 value) 
{
    write_psg(PORT_B, value);
}

