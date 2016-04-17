/* adc_lib.c
 *
 *  Created on: 17.04.2016
 *      Author: JS
 */

#include "main.h"
#include "adc_lib.h"

void adc_configuration()
{
    //First case for simple configuration to measure ext voltage:

    //some gpio configuration:
    //RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= (0x3 << 0);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_InitTypeDef adc;
    ADC_StructInit(&adc);
    adc.ADC_ContinuousConvMode = ENABLE;
    adc.ADC_NbrOfConversion = 1;
    adc.ADC_ExternalTrigConv = 0;
    ADC_Init(ADC1, &adc);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_84Cycles);
    ADC_Cmd(ADC1, ENABLE);

    // ADC_ResetCalibration(ADC1);
    // while (ADC_GetResetCalibrationStatus(ADC1));
    //
    // ADC_StartCalibration(ADC1);
    // while (ADC_GetCalibrationStatus(ADC1));

    ADC_SoftwareStartConv(ADC1);
}
