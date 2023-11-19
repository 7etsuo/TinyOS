/**
 *
 * @brief Exception Vector Assignments for the system.
 * 
 * @details The table outlines the vector assignments for the system, 
 * related to the Motorola 68000 series microprocessor.
 * 
 * | Vector Number(s) | Address (Dec/Hex) | Space | Assignment               |
 * |------------------|-------------------|-------|--------------------------|
 * | 0                | 0/000             | SP    | Reset: Initial SSP       |
 * | 1                | 4/004             | SP    | Reset: Initial PC        |
 * | 2                | 8/008             | SD    | Bus Error                |
 * | 3                | 12/00C            | SD    | Address Error            |
 * | 4                | 16/010            | SD    | Illegal Instruction      |
 * | 5                | 20/014            | SD    | Zero Divide              |
 * | 6                | 24/018            | SD    | CHK Instruction          |
 * | 7                | 28/01C            | SD    | TRAPV Instruction        |
 * | 8                | 32/020            | SD    | Privilege Violation      |
 * | 9                | 36/024            | SD    | Trace                    |
 * | 10               | 40/028            | SD    | Line 1010 Emulator       |
 * | 11               | 44/02C            | SD    | Line 1111 Emulator       |
 * | 12-15            | 48-60/030-03C     | SD    | (Unassigned, Reserved)   |
 * | 16-23            | 64-92/040-05C     | SD    | (Unassigned, Reserved)   |
 * | 24               | 96/060            | SD    | Spurious Interrupt       |
 * | 25               | 100/064           | SD    | Level 1 Interrupt Autovector |
 * | 26               | 104/068           | SD    | Level 2 Interrupt Autovector |
 * | 27               | 108/06C           | SD    | Level 3 Interrupt Autovector |
 * | 28               | 112/070           | SD    | Level 4 Interrupt Autovector |
 * | 29               | 116/074           | SD    | Level 5 Interrupt Autovector |
 * | 30               | 120/078           | SD    | Level 6 Interrupt Autovector |
 * | 31               | 124/07C           | SD    | Level 7 Interrupt Autovector |
 * | 32-47            | 128-191/080-0BF   | SD    | TRAP Instruction Vectors |
 * | 48-63            | 192-255/0C0-0FF   | SD    | (Unassigned, Reserved)   |
 * | 64-255           | 256-1023/100-3FF  | SD    | User Interrupt Vectors   |
 * 
 * @note
 * 1. Vector numbers 12, 13, 16 through 23, and 48 through 63 are reserved for future enhancements by Motorola.
 *    No user peripheral devices should be assigned these numbers.
 * 2. Reset vector (0) requires four words, unlike the other vectors which only require two words,
 *    and is located in the supervisor program space.
 * 3. The spurious interrupt vector is taken when there is a bus error indication during interrupt processing.
 * 4. TRAP #n uses vector number 32 + n.
 * 5. For MC68010/MC68012 only. This vector is unassigned, reserved on the MC68000, and MC68008.
 * 6. SP denotes supervisor program space, and SD denotes supervisor data space.
 */

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "../TYPES.H"

#define HBL_VECTOR	26  /**< Horizontal blank vector. */
#define VBL_VECTOR	28  /**< Vertical blank vector. */
#define TRAP_0_VECTOR	32  /**< System call: reboot. */
#define TRAP_1_VECTOR	33  /**< System call: exit. */
#define TRAP_2_VECTOR	34  /**< System call: create_process. */
#define TRAP_3_VECTOR	35  /**< System call: write. */
#define TRAP_4_VECTOR	36  /**< System call: read. */
#define TRAP_5_VECTOR	37  /**< System call: get_pid. */
#define TRAP_7_VECTOR	39  /**< System call: yield. */
#define IKBD_VECTOR 	70  /**< IKBD vector. */
#define TIMER_A_VECTOR	77  /**< Timer A vector. */

/**
 * @defgroup InterruptHandling Interrupt Handling Variables and Functions
 * 
 * @brief Variables and functions related to handling interrupts.
 * 
 * @{
 */
/** 
 * @brief Pointer to the system's vector table. 
 */
extern Vector * const vector_table;

/** 
 * @brief Initializes the system's vector table.
 *
 * This function sets up the default exception handlers for the system.
 */
void init_vector_table(void);

/** @} */
#endif /* INTERRUPT_H */

