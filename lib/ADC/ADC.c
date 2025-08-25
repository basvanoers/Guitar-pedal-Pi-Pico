#include "ADC.h"
#include <stdio.h>
#include "hardware/adc.h"

#include <math.h>

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

#define  NUM_SAMPLES  16

float Sample_buffer[NUM_SAMPLES];


int ADC_init(int CAPTURE_CHANNEL, int CLOCK_DIV)
{
   adc_gpio_init(26 + CAPTURE_CHANNEL);
    adc_init();
    adc_select_input(CAPTURE_CHANNEL);
    /*adc_fifo_setup(
        true,
        true,
        1,
        true,
        false
    );

    adc_set_clkdiv(CLOCK_DIV);

    uint dma_adc_chan = dma_claim_unused_channel(true);
    dma_channel_config dma_adc_cfg =  dma_channel_get_default_config(dma_adc_chan);
    channel_config_set_transfer_data_size(&dma_adc_cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&dma_adc_cfg, false);
    channel_config_set_write_increment(&dma_adc_cfg,true);
    channel_config_set_dreq(&dma_adc_cfg, DREQ_ADC);

    dma_channel_configure(dma_adc_chan, &dma_adc_cfg,
    Sample_buffer,
    &adc_hw->fifo,
    NUM_SAMPLES,
    false
    
    );*/

   // adc_run(true);
    return 1;
   
}
float ADC_sample()
{
    
/*
   float sample;
   for(int i =0;i<NUM_SAMPLES;i++)
   {
    sample += Sample_buffer[i];
   }    
   sample = sample/4;

*/
float sample = adc_read();
    return sample;

        
}