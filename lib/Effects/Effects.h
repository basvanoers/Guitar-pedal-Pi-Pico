#include <stdio.h>
#include <stdint.h>





extern int n ;
extern float c;





void effect_init(const int GPIO_PIN);
float Gain(float sample,float gain);
float Delay(float sample,float tijd);
float LFO(float freq,float amp);
float Tremelo(float sample, float freq);

