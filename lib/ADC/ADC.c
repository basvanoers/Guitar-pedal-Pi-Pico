#include "ADC.h"
#include <stdio.h>
#include "hardware/adc.h"

#include <math.h>

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

//#define CLOCK_DIV 960
#define FSAMP 50000

// Channel 0 is GPIO26
//#define CAPTURE_CHANNEL 0
#define LED_PIN 25
#define NSAMP 1000

dma_channel_config cfg;
uint dma_chan;


void ADC_init(int CAPTURE_CHANNEL, int CLOCK_DIV)
{
    stdio_init_all();
    adc_init();
    adc_gpio_init(26 + CAPTURE_CHANNEL);
   

}
float ADC_sample()
{
    float sample  =adc_read();
    
    return sample;

        
}