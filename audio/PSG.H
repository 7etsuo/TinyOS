#ifndef PSG_H
#define PSG_H

#include "../types.h"

/*
 *  Envelope control register
 *  shapes, and sustain register bit values
 *  
 *           B3  B2  B1   B0
 *          CONT ATT ALT HOLD
 * 
 *            0   0   x   x  \_______________________      0x00
 *            0   1   x   x  / |_____________________      0x04
 *            1   0   0   0  \ |\ |\ |\ |\ |\ |\ |\        0x08
 *            1   0   0   1  \_______________________      0x09
 *            1   0   1   0  \  /\  /\  /\  /\  /\  /      0x0A
 *            1   0   1   1  \ |--------------------       0x0B
 *            1   1   0   0  / | /| /| /| /| /| /| /       0x0C
 *            1   1   0   1  / ______________________      0x0D
 *            1   1   1   0  / \  /\  /\  /\  /\  /\       0x0E
 *            1   1   1   1  / |_____________________      0x0F
 */

#define IDE_RESET_ON 0x01
#define IDE_RESET_OFF 0xFE
#define INTERNAL_SPEAKER_ON 0x02
#define INTERNAL_SPEAKER_OFF 0xFD
#define CENTRONICS_STROBE_ON 0x04
#define CENTRONICS_STROBE_OFF 0xFB
#define RESET_DSP_ON 0x08
#define RESET_DSP_OFF 0xF7

typedef enum {
    R0=0x0, R1, R2, R3, R4, R5, R6, R7, R8, R9, RA, RB, RC, RD, RE, RF, PORT_A = 0xE, PORT_B = 0xF
} Register;

typedef enum { A, B, C } Channel;

static const UINT8 IO_CONFIGS[] = { 0x36, 0x37, 0x3E, 0x2D, 0x2F, 0x3D, 0x1B, 0x1F, 0x3B, 0x3F };
typedef enum { A_NOISEON_TONEON=0, A_NOISEON_TONEOFF, A_NOISEOFF_TONEON, B_NOISEON_TONEON, B_NOISEON_TONEOFF, B_NOISEOFF_TONEON, C_NOISEON_TONEON, C_NOISEON_TONEOFF, C_NOISEOFF_TONEON, OFF } IOConfig;

static const UINT8 ENV_SHAPES[] = { 0x00, 0x04, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
typedef enum { OFF_ATT_OFF=0, OFF_ATT_ON, ON_ATT_OFF_ALT_OFF_HOLD_OFF, ON_ATT_OFF_ALT_OFF_HOLD_ON, ON_ATT_OFF_ALT_ON_HOLD_OFF, ON_ATT_OFF_ALT_ON_HOLD_ON, ON_ATT_ON_ALT_OFF_HOLD_OFF, ON_ATT_ON_ALT_OFF_HOLD_ON, ON_ATT_ON_ALT_ON_HOLD_OFF, ON_ATT_ON_ALT_ON_HOLD_ON } EnvShape;

static const Register FINE_TONE_REGS[] = { R0, R2, R4 };
static const Register ROUGH_TONE_REGS[] = { R1, R3, R5 };
static const Register VOLUME_REGS[] = { R8, R9, RA };
static const IOConfig CONFIGS[] = { A_NOISEON_TONEON, A_NOISEON_TONEOFF, A_NOISEOFF_TONEON, B_NOISEON_TONEON, B_NOISEON_TONEOFF, B_NOISEOFF_TONEON, C_NOISEON_TONEON, C_NOISEON_TONEOFF, C_NOISEOFF_TONEON };


extern IO_PORT8 PSG_reg_select;
extern IO_PORT8 PSG_reg_write; 

/**
 * @brief Write data to a PSG register.
 *
 * @param reg Register to write to.
 * @param val Value to write.
 */
void write_psg(Register reg, UINT8 val);

/**
 * @brief Read data from a PSG register.
 *
 * @param reg Register to read from.
 * @return The read value.
 */
UINT8 read_psg(Register reg);

/**
 * @brief Set the frequency for a specific channel.
 *
 * @param channel Channel to set.
 * @param tuning Frequency value.
 */
void set_tone(Channel channel, UINT16 tuning);

/**
 * @brief Set the volume for a specific channel.
 *
 * @param channel Channel to set.
 * @param volume Volume level.
 */
void set_volume(Channel channel, UINT8 volume);

/**
 * @brief Enable or disable tone and noise on a channel.
 *
 * @param channel Channel to configure.
 * @param tone_on Enable tone.
 * @param noise_on Enable noise.
 */
void enable_channel(Channel channel, UINT8 tone_on, UINT8 noise_on);

/**
 * @brief Turn off sound for all channels.
 */
void stop_sound();

/**
 * @brief Set noise parameters.
 *
 * @param tuning Noise frequency.
 */
void set_noise(UINT8 tuning);

/**
 * @brief Configure envelope for sound modulation.
 *
 * @param shape Envelope shape.
 * @param sustain Sustain level.
 */
void set_envelope(EnvShape shape, UINT16 sustain);

/**
 * @brief Control Port A configurations.
 *
 * @param mask Mask for setting the configurations.
 * @param value Values to set.
 */
void control_portA(UINT8 mask, UINT8 value);

/**
 * @brief Control Port B configurations.
 *
 * @param value Values to set.
 */
void control_portB(UINT8 value);

#endif /* PSG_H */
