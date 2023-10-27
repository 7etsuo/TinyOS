#include "io.h"
#include "../hal/mfp.h"
#include "../audio/midi.h"
#include "../keyboard/ikbd.h"

void init_IO()
{
	/* note: memory & video already configured (in start.s)              */
	/*       ACIAs require software reset                                */
	/*       all other controllers have been hardware reset (in start.s) */

	*MFP_VR = 0x48;				/* Set Vector Register for automatic end-of-interrupt handling */

	*MFP_TACR = 0x07;			/* delay mode w/ 200 prescaler on 2.4576 MHz */
	*MFP_TADR = 0x00;			/* counts down from 256 -> 48 timeouts/s */

	*IKBD_ACIA_CR = 0x17;		/* master reset */
	*IKBD_ACIA_CR = 0x96;		/* 8N1, Tx IRQs disabled, Rx IRQs enabled, clock divide down by 64 */

	*MIDI_ACIA_CR = 0x57;		/* master reset  */
	*MIDI_ACIA_CR = 0x55;		/* IRQs disabled (other settings would need to be checked before use */

	*MFP_IERA |= MFP_TIMER_A;
	*MFP_IMRA |= MFP_TIMER_A;

	*MFP_IERB |= MFP_GPIP4;
	*MFP_IMRB |= MFP_GPIP4;
}

