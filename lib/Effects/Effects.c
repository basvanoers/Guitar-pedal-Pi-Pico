#include "Effects.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "hardware/pwm.h"
#include  "PWM.h"
#include "pico/stdlib.h"





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