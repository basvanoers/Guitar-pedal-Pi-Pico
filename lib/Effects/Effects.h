#include <stdio.h>
#include <stdint.h>




extern float sine[];
extern int k;
extern int m;
extern int n ;
extern int c;
extern int q;
extern int s;



float tremelo(float sample, float freq);
float gain(float sample,float gain);
float hard_clipping_distortion(float sample,float clipping);
float soft_clipping_distortion(float sample,float clipping);
void effect_init(const int GPIO_PIN);

