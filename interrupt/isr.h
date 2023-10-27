#ifndef ISR_H
#define ISR_H

#include "../types.h"

/**
 * @defgroup InterruptVectors Interrupt Request Vectors
 * 
 * @brief Exception vectors corresponding to interrupts.
 * 
 * The table outlines the exception vectors used for various interrupts.
 * 
 * @{
 */


/** @} */

/** 
 * @brief Counter for vertical blank interrupts. 
 */
extern UINT16 * const vbl_counter;

/**
 * @brief keyboard state
 *
 * 0=not in mouse packet, 1=expecting delta x, 2=expecting delta y 
 */
extern UINT16 * const kybd_isr_state; 

/** 
 * @brief Interrupt service routine for vertical blank interrupt.
 */
extern void vbl_isr();

/** 
 * @brief Interrupt service routine for Timer A. 
 */
extern void timer_A_isr();

extern void ikbd_isr();
extern void exception_isr();
extern void addr_exception_isr();

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
void do_exception_isr(UINT16 sr);
void do_addr_exception_isr(UINT16 flags, UINT32 addr, UINT16 ir, UINT16 sr);
void do_ikbd_isr();

#endif /* ISR_H */
