#ifndef MAIN_H_
#define MAIN_H_

// main includes
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx_conf.h"


#define BUFFERSIZE  20000 // 1k samples/s  hence 20s for measurement
__IO uint16_t ADC_results[BUFFERSIZE];  // buffer for ADC results 
__IO uint8_t endOfMeasurement;

uint8_t part1;
uint8_t part2;
uint8_t firstDMA;


void init_board();

// DMA prototypes
void dma_it_conf(void);
void dma_send_bt(uint32_t BaseAddr, uint32_t size);

extern void delay_ms(volatile uint32_t milisecs);

inline void blinkBlink()
{
    for(int x = 0; x < 100; x++)
    {
        delay_ms(50);
        GPIOD->ODR ^= (0xF << 12);
    }
}


#endif /* MAIN_H_ */
