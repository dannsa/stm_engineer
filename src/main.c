#include "main.h"
#include "systick.h"
#include "uart_for_bt.h"
#include "adc_lib.h"
#include "semihosting.h"

int main(void)
{
    time_period = 500;
#ifdef USE_DEBUG
    setbuf(stdout, NULL);
#endif

    init_board();
    usart2_configuration();
    adc_configuration(); //NOT READY
    uint16_t adc_value = 0;

    for(;;)
    {
        delay_ms(time_period);
        GPIOD->ODR ^= (0xF << 12);           // Toggle the pin
        adc_value = ADC_GetConversionValue(ADC1);
        dbg_log("ADC voltage: %.2f V\n", adc_value * 3.3f / 4096.0f);
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
