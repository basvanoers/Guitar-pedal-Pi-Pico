#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include"lib/ADC/ADC.h"
#include <stdio.h>
#include "ADC.h"
#include <stdio.h>
#include "hardware/pwm.h"


#include <math.h>
#include "pico/stdlib.h"
#include "lib/PWM/PWM.h"
#include "lib/I2S/I2S.h"
#include "PWM.pio.h"
#include "I2S.pio.h"
#include"lib/Effects/Effects.h"


#define sample_freq 48000
#define sample_time_us (1000000/48000)







void main(){


    
    stdio_init_all();
    ADC_init(0,96);
    RGB_led_init(13,10,15);
    effect_init(0);





 
while (1) {
    
    
    float sample = ADC_sample(); // get sample and put it in a variable
    sample = sample/4096.0;
    sample -=0.5;
    
    sample+=0.5;
    sample = sample *4096.0;
    output(sample,0);
}
}

    
    
    
    
    
    

    