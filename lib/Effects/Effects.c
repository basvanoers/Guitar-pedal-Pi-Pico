#include "Effects.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "hardware/pwm.h"
#include  "PWM.h"
#include "pico/stdlib.h"

float LFO_array[480] = {240, 243, 246, 249, 253, 256, 259, 262,
265, 268, 271, 274, 278, 281, 284, 287,
290, 293, 296, 299, 302, 305, 308, 311,
314, 317, 320, 323, 326, 329, 332, 335,
338, 340, 343, 346, 349, 352, 355, 357,
360, 363, 365, 368, 371, 373, 376, 379,
381, 384, 386, 389, 391, 393, 396, 398,
401, 403, 405, 407, 410, 412, 414, 416,
418, 420, 422, 425, 427, 428, 430, 432,
434, 436, 438, 440, 441, 443, 445, 446,
448, 449, 451, 452, 454, 455, 457, 458,
459, 461, 462, 463, 464, 465, 466, 467,
468, 469, 470, 471, 472, 473, 473, 474,
475, 475, 476, 477, 477, 478, 478, 478,
479, 479, 479, 479, 480, 480, 480, 480,
480, 480, 480, 480, 480, 479, 479, 479,
479, 478, 478, 478, 477, 477, 476, 475,
475, 474, 473, 473, 472, 471, 470, 469,
468, 467, 466, 465, 464, 463, 462, 461,
459, 458, 457, 455, 454, 452, 451, 449,
448, 446, 445, 443, 441, 440, 438, 436,
434, 432, 430, 428, 427, 425, 422, 420,
418, 416, 414, 412, 410, 407, 405, 403,
401, 398, 396, 393, 391, 389, 386, 384,
381, 379, 376, 373, 371, 368, 365, 363,
360, 357, 355, 352, 349, 346, 343, 340,
338, 335, 332, 329, 326, 323, 320, 317,
314, 311, 308, 305, 302, 299, 296, 293,
290, 287, 284, 281, 278, 274, 271, 268,
265, 262, 259, 256, 253, 249, 246, 243,
240, 237, 234, 231, 227, 224, 221, 218,
215, 212, 209, 206, 202, 199, 196, 193,
190, 187, 184, 181, 178, 175, 172, 169,
166, 163, 160, 157, 154, 151, 148, 145,
142, 140, 137, 134, 131, 128, 125, 123,
120, 117, 115, 112, 109, 107, 104, 101,
99, 96, 94, 91, 89, 87, 84, 82,
79, 77, 75, 73, 70, 68, 66, 64,
62, 60, 58, 55, 53, 52, 50, 48,
46, 44, 42, 40, 39, 37, 35, 34,
32, 31, 29, 28, 26, 25, 23, 22,
21, 19, 18, 17, 16, 15, 14, 13,
12, 11, 10, 9, 8, 7, 7, 6,
5, 5, 4, 3, 3, 2, 2, 2,
1, 1, 1, 1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 1, 1, 1,
1, 2, 2, 2, 3, 3, 4, 5,
5, 6, 7, 7, 8, 9, 10, 11,
12, 13, 14, 15, 16, 17, 18, 19,
21, 22, 23, 25, 26, 28, 29, 31,
32, 34, 35, 37, 39, 40, 42, 44,
46, 48, 50, 52, 53, 55, 58, 60,
62, 64, 66, 68, 70, 73, 75, 77,
79, 82, 84, 87, 89, 91, 94, 96,
99, 101, 104, 107, 109, 112, 115, 117,
120, 123, 125, 128, 131, 134, 137, 140,
142, 145, 148, 151, 154, 157, 160, 163,
166, 169, 172, 175, 178, 181, 184, 187,
190, 193, 196, 199, 202, 206, 209, 212,
215, 218, 221, 224, 227, 231, 234, 237};

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
    int N = (int)(48000/tijd);
    float Output = sample + delay_buffer[n]*c;
    delay_buffer[n] = Output;
    n=(n+1)%N;
    return Output;

}
float Freq_lfo=0;
int counter_lfo=0;

float LFO(float freq,float amp)
{
    Freq_lfo++;

    if (Freq_lfo >=(100/freq))
    {
        counter_lfo++;
        Freq_lfo = 0;
    }
    if (counter_lfo >= 480)
    {
        counter_lfo=0;
    }

    
    return (LFO_array[counter_lfo]/480.0)*amp; 
}
float Tremelo(float sample, float freq)
{

    return sample*LFO(freq,1);
}

float Delay_line_buffer[48000]={0};
int SAMPLES =0;
void delay_line_add(float sample)
{
Delay_line_buffer[SAMPLES] = sample;
SAMPLES++;
if (SAMPLES >=48000)
{
    SAMPLES = 0;
}

}
float delay_line_get(float sample)
{
    float index = SAMPLES-sample;
    if (index<0)
    {
        index =0;
    }
    return Delay_line_buffer[(int)(index)];
}
float a0, a1, a2, b1, b2, hp_in_z1, hp_in_z2, hp_out_z1, hp_out_z2;
float Do_HighPass (float inSample) {
	//300Hz high-pass, 96k
	a0 = 0.9862117951198142f;
	a1 = -1.9724235902396283f;
	a2 = 0.9862117951198142f;
	b1 = -1.972233470205696f;
	b2 = 0.9726137102735608f;

	float inSampleF = (float)inSample;
	float outSampleF =
			a0 * inSampleF
			+ a1 * hp_in_z1
			+ a2 * hp_in_z2
			- b1 * hp_out_z1
			- b2 * hp_out_z2;
	hp_in_z2 = hp_in_z1;
	hp_in_z1 = inSampleF;
	hp_out_z2 = hp_out_z1;
	hp_out_z1 = outSampleF;

	return  outSampleF;
}








float s0,s1,f1,f0;
#define BUFSIZE 1024
float Pitch_buffer[1024] = {0};
int read_index = 0;
int write_index = BUFSIZE/2;
float Pitch_shift(float sample)
{
    sample = Do_HighPass(sample);
    f0 = 0.25;
    f1= 1-f0;

    read_index +=2;
   
    if (read_index >= BUFSIZE)
    {
        read_index =0;
    }
    if (write_index >= BUFSIZE)
    {
        write_index =0;
    }
    Pitch_buffer[write_index] = sample;
s0 = f0 * Pitch_buffer[read_index];
   s1 = f1 * Pitch_buffer[(read_index + BUFSIZE/2) % BUFSIZE];
    write_index +=1;
   return s0 + s1;

}

float Flanger(float sample, float freq)
{
    delay_line_add(sample);
float Output = sample + delay_line_get(LFO(freq,480))*0.8;

    return Output;
}
float Chorus(float sample,float freq)
{
    delay_line_add(sample);
    float Output = sample+delay_line_get(LFO(freq,480))*0.4+delay_line_get(LFO(freq+1,480))*0.4+delay_line_get(LFO(freq+2,480))*0.4+delay_line_get(LFO(freq+3,480))*0.4;
    return Output;
}

float overdrive(float sample, float factor)
{
    sample = Gain(sample,5);
    
   
    if (sample <-factor)
    {
        sample = -factor;
    }
    
    if(sample >factor)
    {
        sample = factor;
    }
   
    return sample;

}
float Vibrato(float sample, float freq)
{
    delay_line_add(sample);
    float Output = delay_line_get(LFO(freq,480));
    return Output;
}
