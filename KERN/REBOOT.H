#ifndef REBOOT_H
#define REBOOT_H

/* Values taken from Linux */

/*
 * [TODO] Implement Magic values required to use _reboot() system call.
 */

#define	TINYOS_REBOOT_MAGIC1	0xfee1dead
#define	TINYOS_REBOOT_MAGIC2	672274793
#define	TINYOS_REBOOT_MAGIC2A	85072278
#define	TINYOS_REBOOT_MAGIC2B	369367448
#define	TINYOS_REBOOT_MAGIC2C	537993216

/*
 * Commands accepted by the _reboot() system call.
 *
 * RESTART     Restart system using default command and mode.
 * HALT        Stop OS and give system control to ROM monitor, if any.
 *             prints "System halted"
 * CAD_ON      Ctrl-Alt-Del sequence causes RESTART command.
 * CAD_OFF     Ctrl-Alt-Del sequence sends SIGINT to init task.
 * POWER_OFF   Stop OS and remove all power from system, if possible.
 *             prints "Power down"
 * RESTART2    Restart system using given command string.
 *             prints "Restarting system"
 * SW_SUSPEND  Suspend system using software suspend if compiled in.
 * KEXEC       Restart system using a previously loaded Linux kernel
 */
#define	TINYOS_REBOOT_CMD_RESTART	0x01234567
#define	TINYOS_REBOOT_CMD_HALT		0xCDEF0123
#define	TINYOS_REBOOT_CMD_CAD_ON    	0x89ABCDEF
#define	TINYOS_REBOOT_CMD_CAD_OFF   	0x00000000
#define	TINYOS_REBOOT_CMD_POWER_OFF     0x4321FEDC
#define	TINYOS_REBOOT_CMD_RESTART2	0xA1B2C3D4
#define	TINYOS_REBOOT_CMD_SW_SUSPEND	0xD000FCE2
#define	TINYOS_REBOOT_CMD_KEXEC		0x45584543

extern void restart(void);

#endif /* REBOOT_H */

