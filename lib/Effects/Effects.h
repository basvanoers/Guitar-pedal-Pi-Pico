#include <stdio.h>
#include <stdint.h>





extern int n ;
extern float c;





void effect_init(const int GPIO_PIN);
float Gain(float sample,float gain);
float Delay(float sample,float tijd);
float LFO(float freq,float amp);
float Tremelo(float sample, float freq);
void delay_line_add(float sample);
float delay_line_get(float sample);
float Flanger(float sample, float freq);
float Chorus(float sample,float freq);
float overdrive(float sample, float factor);
float Vibrato(float sample, float freq);
float freq(float sample,float freq);
float Pitch_shift(float sample, int shift);
float All_pass_process(float sample, float k);
float Phaser(float sample, float Freq);

