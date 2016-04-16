/* systick.h
 *
 *  Created on: 01.04.2016
 *      Author: JS
 */

 #ifndef SYSTICK_H
 #define SYSTICK_H

// not optimized variable for systick timer
volatile uint32_t sys_ticks;

// PROTOTYPES
void ticks_handler();
void delay_ms(volatile uint32_t milisecs);

#endif /* SYSTICK_H */
