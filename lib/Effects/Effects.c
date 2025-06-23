#include "Effects.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "hardware/pwm.h"
#include  "PWM.h"
#include "pico/stdlib.h"




float sine[] = {0.00, 0.03, 0.06, 0.10, 0.13, 0.16, 0.19, 0.22, 0.25, 0.28, 0.31, 0.34, 0.37, 0.40, 0.43, 0.46, 0.49, 0.51, 0.54, 0.57, 0.59, 0.62, 0.64, 0.67, 0.69, 0.71, 0.73, 0.76, 0.78, 0.80, 0.81, 0.83, 0.85, 0.87, 0.88, 0.90, 0.91, 0.92, 0.93, 0.95, 0.95, 0.96, 0.97, 0.98, 0.98, 0.99, 0.99, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.99, 0.99, 0.98, 0.98, 0.97, 0.96, 0.95, 0.95, 0.93, 0.92, 0.91, 0.90, 0.88, 0.87, 0.83, 0.81, 0.80, 0.78, 0.76, 0.73, 0.71, 0.69, 0.67, 0.64, 0.62, 0.59, 0.57, 0.54, 0.51, 0.49, 0.46, 0.43, 0.40, 0.37, 0.34, 0.31, 0.28, 0.25, 0.22, 0.19, 0.16, 0.13, 0.10, 0.06, 0.03, 0.00};
int k=0;
int m=0;
int n=0;
int c=3;



float tremelo(float sample, float freq) {
    k++;
    
    if (k >= 480.0 / freq) {
        k = 0;
        m++;
        if (m == 99) {
            m = 0;
        }
    }
    return sample * sine[m];
}




float gain(float sample,float gain)
{
    return sample*gain;
}





float soft_clipping_distortion(float sample,float clipping){
    
    
    if (sample>0.0)
    {
        sample = -1+ exp(sample+clipping);

    }
    if(sample <0.0)
    {
        sample = 1-exp(-sample+clipping);
    } 
    return sample;
}



float hard_clipping_distortion(float sample, float clipping){
    printf("%f\n",sample);
    if (sample>clipping)
    {
        sample =clipping;
    }
    if (sample<-clipping)
    {
        sample =-clipping;

    }
    sample = gain(sample,2.5);
    return sample;
}
void effect_init(const int GPIO_PIN)
{
    pwm_init_pin(GPIO_PIN);

    int slice = pwm_gpio_to_slice_num(GPIO_PIN);
    int channel = pwm_gpio_to_channel(GPIO_PIN);
    
    gpio_init(12);
    gpio_set_dir(12, 0);
    gpio_init(11);
    gpio_set_dir(11, 0);
    gpio_pull_up(11);
    gpio_pull_up(12);
    gpio_init(4);
    gpio_set_dir(4, 1);
    gpio_pull_down(4);
}