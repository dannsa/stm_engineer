
#include "main.h"
#include "systick.h"
#include "uart_for_bt.h"

int main(void)
{   time_period = 100;
    init_board();
    usart2_configuration();
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

    // YAYA with stdperiph ...
    // Configure PD12, PD13, PD14 and PD15 in output pushpull mode
    // GPIO_InitTypeDef  GPIO_InitStructure;
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    // GPIO_Init(GPIOD, &GPIO_InitStructure);
}

// Dummy for compilation
void _init() {}
