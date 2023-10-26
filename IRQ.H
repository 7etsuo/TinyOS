#ifndef INTERRUPT_H
#define INTERRUPT_H

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

#include "types.h"

/**
 * @defgroup InterruptVectors Interrupt Request Vectors
 * 
 * @brief Exception vectors corresponding to interrupts.
 * 
 * The table outlines the exception vectors used for various interrupts.
 * 
 * @{
 */

#define HBL_VECTOR	26    /**< Horizontal blank vector. */
#define VBL_VECTOR	28    /**< Vertical blank vector. */
#define IKBD_VECTOR	70    /**< IKBD vector. */
#define TIMER_A_VECTOR	77  /**< Timer A vector. */

/** @} */

/**
 * @defgroup InterruptHandling Interrupt Handling Variables and Functions
 * 
 * @brief Variables and functions related to handling interrupts.
 * 
 * @{
 */

/** 
 * @brief Counter for vertical blank interrupts. 
 */
extern UINT16 * const vbl_counter;

/** 
 * @brief Pointer to the system's vector table. 
 */
extern Vector * const vector_table;

/** 
 * @brief Interrupt service routine for vertical blank interrupt.
 */
extern void vbl_isr();

/** 
 * @brief Interrupt service routine for Timer A. 
 */
extern void timer_A_isr();

/**
 * @brief Vertical Blank Interrupt Service Routine (VBL ISR) handler.
 *
 * This function is invoked to handle tasks associated with the vertical blank interrupt.
 * It checks and processes queued keyboard auto-repeat inputs, and inverts the cursor state periodically.
 *
 * @note Consideration should be given to masking less aggressively in the future.
 */
void do_vbl_isr();

/**
 * @brief Handles the Timer A interrupt service routine.
 * 
 * This ISR is invoked in response to a Timer A interrupt. The function performs several operations,
 * including checking and updating the state of the current process, and clearing the interrupt request.
 * 
 * @param sr The status register value at the time of the interrupt (Unused) 
 * 
 * @note The Timer A interrupt has a very high priority. Consideration should be given to using a lower 
 *       priority interrupt source, such as Timer C, in the future.
 * @note The 68000's Interrupt Priority Level (IPL) could potentially be lowered to 4 to allow higher 
 *       priority IRQs to preempt this ISR.
 */
void do_timer_A_isr(UINT16 sr);

/** 
 * @brief Initializes the system's vector table.
 *
 * This function sets up the default exception handlers for the system.
 */
void init_vector_table();

/** @} */
#endif /* INTERRUPT_H */

