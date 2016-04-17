#ifndef MAIN_H_
#define MAIN_H_

// main includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "stm32f4xx_conf.h"

uint16_t time_period;
// Start the board clocks, leds etc.
void init_board();

#endif /* MAIN_H_ */
