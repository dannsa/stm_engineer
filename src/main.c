#include "main.h"
#include "systick.h"
#include "uart_for_bt.h"
#include "adc_lib.h"
//#include "semihosting.h"

/*********************************DMA_CONFIGURATION*********************************************/

void dma_it_conf(void)
{
    NVIC_InitTypeDef dma_it_conf;

    // Interrupts for dma
    dma_it_conf.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    dma_it_conf.NVIC_IRQChannelPreemptionPriority = 0;
    dma_it_conf.NVIC_IRQChannelSubPriority = 0;
    dma_it_conf.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&dma_it_conf);
}

void dma_bluetooth_tx(void)
{
    DMA_InitTypeDef	dma_bt;
    //Deinit struccture basic memset
    DMA_StructInit(&dma_bt);
    // Binded to USART 3_TX
    dma_bt.DMA_Channel = DMA_Channel_7;
    // Destination
    dma_bt.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
    // mode to peripheral
    dma_bt.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    // Don't increment register address
    dma_bt.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // Memory increment
    dma_bt.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //Data sizes
    dma_bt.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma_bt.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    // Without circular mode
    dma_bt.DMA_Mode = DMA_Mode_Normal;
    // High priorty for transmision
    dma_bt.DMA_Priority = DMA_Priority_VeryHigh;
    dma_bt.DMA_FIFOMode = DMA_FIFOMode_Disable;
    dma_bt.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    // By single memory burst
    dma_bt.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma_bt.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    // Initialization
    DMA_Init(DMA1_Stream4, &dma_bt);
    // Interrupt enable
	  USART_DMACmd(USART3, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
}

void dma_send_bt(uint32_t BaseAddr, uint32_t size)
{
	  // Wait for transmission completion
    if (firstDMA == 0)
    {
        while (DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4) != SET){};
    }
    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);

    // Source by register
    DMA1_Stream4->M0AR = BaseAddr;
    DMA1_Stream4->NDTR = size;

    // Enable
    DMA1_Stream4->CR |= (uint32_t)DMA_SxCR_EN;
    firstDMA = 0;
}

/*********************************BOARD_CONFIGURATION*********************************************/
void init_board()
{
    // ---------- SysTick timer -------- //
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        while (1){};
    }

    //Clock enable
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    __asm("dsb");  //Stall after clock setup

    // LEDs
    GPIOD->MODER |=  0x55 << 24;
    GPIO_InitTypeDef GPIO_InitStructure;

    // PC1 for ADC input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*********************************MAIN PROGRAM *********************************************/

int main(void)
{
    // Used for semihosting debug
    #ifdef USE_DEBUG
        //setbuf(stdout, NULL);
    #endif
     // Basic GPIO and clocks enabling
    init_board();
    // Global flag for measurmenet duration
    endOfMeasurement = 0;

    // USART3 configuration
    bluetooth_config();
    delay_ms(6000); // sleep for preparation
    // 1ms ADC trigger timer configuration
    ADC_TIMER_trigger_conf();
    // DMA for reading ADC results
    ADC_DMA_config();
    // Interupts for DMA half and finished tr.
    dma_it_conf();
    // DMA tx for bluetooth transmission
    dma_bluetooth_tx();
    // ADC configuration
    ADC_Configuration();
    //Start conversion
    ADC_SoftwareStartConv(ADC1);
    // Enable DMA reading from ADC
    DMA_Cmd(DMA2_Stream0, ENABLE);
    blinkBlink();
    //delay_ms(3000); // sleep for preparation

    delay_ms(50);

    // main loop
    while(1)
    {
        if (endOfMeasurement == 1) // flag marker at the end of DMA transmission after 20s
        {
            // visual signal
            delay_ms(100);
            GPIOD->ODR ^= (0xF << 12);

            for (int i = 0; i < BUFFERSIZE; i++)
            {
                xuart_putNumber(USART3, (ADC_results[i]));
                delay_ms(1);
                GPIOD->ODR ^= (0xF << 12); // fast blinking during transmission
            }

            endOfMeasurement = 0; // disable transmission
        }

        // visual indication
        delay_ms(1000);
        GPIOD->ODR ^= (0xF << 12);
    };


}

// Dummy for compilation
void _init() {}
