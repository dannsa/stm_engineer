/* systick.c
 *
 *  Created on: 01.04.2016
 *      Author: JS
 */
#include "main.h"
#include "systick.h"

 // Called for every systick timer expiry
void ticks_handler()
{
    if (sys_ticks) sys_ticks--;
}

// Delay function for time measurements
void delay_ms(volatile uint32_t milisecs)
{
    sys_ticks = milisecs;
    while(sys_ticks){};
}
