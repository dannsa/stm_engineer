/* adc_lib.c
 *
 *  Created on: 17.04.2016
 *      Author: JS
 */

#include "main.h"
#include "adc_lib.h"

void ADC_Configuration(void)
{
    ADC_CommonInitTypeDef adc_common;
    ADC_InitTypeDef adc;

    // Common ADC structure init, prescalers, delays
    adc_common.ADC_Mode = ADC_Mode_Independent;
    adc_common.ADC_Prescaler = ADC_Prescaler_Div2;
    adc_common.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    adc_common.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&adc_common);

    // Resolution
    adc.ADC_Resolution = ADC_Resolution_12b;
    // Conversion like amplifier
    adc.ADC_ScanConvMode = DISABLE;
    // Continuous mode disabled because of timer
    adc.ADC_ContinuousConvMode = DISABLE;
    // Timer triggering conversion
    adc.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    // Put data in least significant part of uint16_t
    adc.ADC_DataAlign = ADC_DataAlign_Right;
    //Signle conversion
    adc.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &adc);
    // ADC1 channel 11 GPIOC_1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_15Cycles);
    // ADC for DMA configuration
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    // Enable DMA requests
    ADC_DMACmd(ADC1, ENABLE);
    // enable ADC module
    ADC_Cmd(ADC1, ENABLE);
}

void ADC_DMA_config(void)
{

    DMA_InitTypeDef dma_rx;

    // Channle for ADC1 PC1
    dma_rx.DMA_Channel = DMA_Channel_0;
    // Destination
    dma_rx.DMA_Memory0BaseAddr = (uint32_t)&ADC_results[0];
    // Source
    dma_rx.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    // Mode & count
    dma_rx.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dma_rx.DMA_BufferSize = BUFFERSIZE; 
    // Inc in memory, iterate over buff
    dma_rx.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma_rx.DMA_MemoryInc = DMA_MemoryInc_Enable;
    // Half word transfer
    dma_rx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma_rx.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    // Normal mode single
    dma_rx.DMA_Mode = DMA_Mode_Normal;
    dma_rx.DMA_Priority = DMA_Priority_High;
    dma_rx.DMA_FIFOMode = DMA_FIFOMode_Enable;
    dma_rx.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    // Per one package
    dma_rx.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma_rx.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    // Init
    DMA_Init(DMA2_Stream0, &dma_rx);
    //Enable Interrupts
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC , ENABLE);
}

void ADC_TIMER_trigger_conf(void)
{
  TIM_TimeBaseInitTypeDef base_time;
  TIM_OCInitTypeDef timer;

  // Base configuratiion for periods
  base_time.TIM_Prescaler = (SystemCoreClock / 1000000) - 1;
  base_time.TIM_Period = 1000 - 1; // 1khz
  base_time.TIM_ClockDivision = 0;
  base_time.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &base_time);

  // PWM for impulses
  timer.TIM_OCMode = TIM_OCMode_PWM1;

  timer.TIM_OutputState = TIM_OutputState_Enable;
  timer.TIM_OutputNState = TIM_OutputNState_Disable;
  timer.TIM_Pulse = 10; // not necessary

  // Timer states for PWM
  timer.TIM_OCPolarity = TIM_OCPolarity_High;
  timer.TIM_OCNPolarity = TIM_OCNPolarity_High;
  timer.TIM_OCIdleState = TIM_OCIdleState_Reset;
  timer.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

  // Init
  TIM_OC1Init(TIM1, &timer);

  // Out enable
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  TIM_Cmd(TIM1, ENABLE);
}