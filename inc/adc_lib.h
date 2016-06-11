/* adc_lib.h
 *
 *  Created on: 17.04.2016
 *      Author: JS
 */

 #ifndef ADC_LIB_H
 #define ADC_LIB_H

#include "stm32f4xx_conf.h"
// Configuration
void ADC_Configuration();
void ADC_DMA_config(void);
void ADC_TIMER_trigger_conf(void);

#endif /* ADC_LIB_H */
