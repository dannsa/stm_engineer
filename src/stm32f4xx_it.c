#include "stm32f4xx_it.h"
#include "systick.h"
#include "main.h"
#include "semihosting.h"
#include "uart_for_bt.h"

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    ticks_handler();
}


void USART3_IRQHandler(void)
{
    char ch = xuart_getChar(USART3);
    if(ch)
    {
        //startMeas = 1;
    }
}

void DMA2_Stream0_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_HTIF0))
    {
        DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_HTIF0);

        //dma_send_bt((uint32_t)&partADC[0], 1);
        //delay_ms(1);
        //dma_send_bt((uint32_t)&ADCConvertedValues[0], BUFFERSIZE);

        //GPIOD->ODR ^= (0xF << 12);
    }
 
    // Transfer complete
    if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
    {
        // Clear it bit
        DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);

        DMA_Cmd(DMA2_Stream0, DISABLE);
        //dma_send_bt((uint32_t)&ADCConvertedValues[0], BUFFERSIZE*2);
        endOfMeasurement = 1;
        GPIOD->ODR ^= (0xF << 12);
    }
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
