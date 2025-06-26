#include "Effects.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "hardware/pwm.h"
#include  "PWM.h"
#include "pico/stdlib.h"

float TREMELO[200] = {2, 4, 6, 8, 10, 12, 14, 16,
18, 20, 22, 24, 26, 28, 30, 32,
34, 36, 38, 40, 42, 44, 46, 48,
50, 52, 54, 56, 58, 60, 62, 64,
66, 68, 70, 72, 74, 76, 78, 80,
82, 84, 86, 88, 90, 92, 94, 96,
98, 100, 102, 104, 106, 108, 110, 112,
114, 116, 118, 120, 122, 124, 126, 128,
130, 132, 134, 136, 138, 140, 142, 144,
146, 148, 150, 152, 154, 156, 158, 160,
162, 164, 166, 168, 170, 172, 174, 176,
178, 180, 182, 184, 186, 188, 190, 192,
194, 196, 198, 200, 198, 196, 194, 192,
190, 188, 186, 184, 182, 180, 178, 176,
174, 172, 170, 168, 166, 164, 162, 160,
158, 156, 154, 152, 150, 148, 146, 144,
142, 140, 138, 136, 134, 132, 130, 128,
126, 124, 122, 120, 118, 116, 114, 112,
110, 108, 106, 104, 102, 100, 98, 96,
94, 92, 90, 88, 86, 84, 82, 80,
78, 76, 74, 72, 70, 68, 66, 64,
62, 60, 58, 56, 54, 52, 50, 48,
46, 44, 42, 40, 38, 36, 34, 32,
30, 28, 26, 24, 22, 20, 18, 16,
14, 12, 10, 8, 6, 4, 2, 0};

int i =0;

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

float Gain(float sample, float gain)
{
    sample = sample *gain;
    return sample;

}
int n = 0;
float c=0.5;
float delay_buffer[48000] = {0};
float Delay(float sample,float tijd)
{
    int N = 48000/tijd;
    float Output = sample + delay_buffer[n]*c;
    delay_buffer[n] = Output;
    n=(n+1)%N;
    return Output;

}


