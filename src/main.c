#include "main.h"
#include "systick.h"
#include "uart_for_bt.h"
#include "adc_lib.h"

int main(void)
{   time_period = 100;
    init_board();
    usart2_configuration();
    //adc_configuration(); NOT READY
    for(;;)
    {
        delay_ms(time_period);
        GPIOD->ODR ^= (0xF << 12);           // Toggle the pin
    }
    return 0;
}

void init_board()
{
    // ---------- SysTick timer -------- //
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        while (1){};
    }

    //Clock enable
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    __asm("dsb");  //Stall after clock setup

    // GPIOD P12-15 output mode
    GPIOD->MODER |=  0x55 << 24;
}

// Dummy for compilation
void _init() {}
