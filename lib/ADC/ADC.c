#include "ADC.h"
#include <stdio.h>
#include "hardware/adc.h"

#include <math.h>

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

;


void ADC_init(int CAPTURE_CHANNEL, int CLOCK_DIV)
{
    stdio_init_all();
    adc_init();
    adc_gpio_init(26 + CAPTURE_CHANNEL);
    adc_set_clkdiv(1);
   

}
float ADC_sample()
{
    float sample  =adc_read();
    
    return sample;

        
}