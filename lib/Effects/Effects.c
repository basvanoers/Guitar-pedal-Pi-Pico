#include "Effects.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "hardware/pwm.h"
#include  "PWM.h"
#include "pico/stdlib.h"

float LFO_array[480] = {2, 4, 6, 8, 10, 12, 14, 16,
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
194, 196, 198, 200, 202, 204, 206, 208,
210, 212, 214, 216, 218, 220, 222, 224,
226, 228, 230, 232, 234, 236, 238, 240,
242, 244, 246, 248, 250, 252, 254, 256,
258, 260, 262, 264, 266, 268, 270, 272,
274, 276, 278, 280, 282, 284, 286, 288,
290, 292, 294, 296, 298, 300, 302, 304,
306, 308, 310, 312, 314, 316, 318, 320,
322, 324, 326, 328, 330, 332, 334, 336,
338, 340, 342, 344, 346, 348, 350, 352,
354, 356, 358, 360, 362, 364, 366, 368,
370, 372, 374, 376, 378, 380, 382, 384,
386, 388, 390, 392, 394, 396, 398, 400,
402, 404, 406, 408, 410, 412, 414, 416,
418, 420, 422, 424, 426, 428, 430, 432,
434, 436, 438, 440, 442, 444, 446, 448,
450, 452, 454, 456, 458, 460, 462, 464,
466, 468, 470, 472, 474, 476, 478, 480,
478, 476, 474, 472, 470, 468, 466, 464,
462, 460, 458, 456, 454, 452, 450, 448,
446, 444, 442, 440, 438, 436, 434, 432,
430, 428, 426, 424, 422, 420, 418, 416,
414, 412, 410, 408, 406, 404, 402, 400,
398, 396, 394, 392, 390, 388, 386, 384,
382, 380, 378, 376, 374, 372, 370, 368,
366, 364, 362, 360, 358, 356, 354, 352,
350, 348, 346, 344, 342, 340, 338, 336,
334, 332, 330, 328, 326, 324, 322, 320,
318, 316, 314, 312, 310, 308, 306, 304,
302, 300, 298, 296, 294, 292, 290, 288,
286, 284, 282, 280, 278, 276, 274, 272,
270, 268, 266, 264, 262, 260, 258, 256,
254, 252, 250, 248, 246, 244, 242, 240,
238, 236, 234, 232, 230, 228, 226, 224,
222, 220, 218, 216, 214, 212, 210, 208,
206, 204, 202, 200, 198, 196, 194, 192,
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


const int NL = 5;
const float NUM[5] = {
     0.9708778522795,   -3.883511409118,    5.825267113677,   -3.883511409118,
     0.9708778522795
};
const int DL = 5;
const float DEN[5] = {
                   1,   -3.940892858927,    5.824419141289,    -3.82612983221,
     0.9426038040469
};
float buffer[5]= {0};
    float buffery[5]={0};
    float output1 =0;
    float  youtput =0;
    int N=3;
    int M= 3;


float IIR_filter_300Hz(float sample)
{
 buffer[0] = sample;

        output1 = 0;
        youtput=0;
        int k=0;
        for (k=0;k <=N;k++)
        {
            output1 = output1 + NUM[k]*buffer[k];

        }
        int r=0;
        for (r =1; r<=M;r++)
        {
           youtput = youtput + DEN[r]*buffery[r];

        }
        int i =N;
        int a =M;
        output1 -=youtput;
        buffery[0] = output1;
        for (a =M;a>=1;a--)
        {
            buffery[a]= buffery[a-1];

        }
        for(i =N; i>=1;i--)
        {
            buffer[i]= buffer[i-1];

        }
        return output1;
}


#define BUFFSIZE 1024
float pitch_shift_array[BUFFSIZE] = {0};
int index_pitch_shift_read = 0;
int index_pitch_shift_write = BUFFSIZE/2;
float s1=0;
float s0=0;
float f0=0.2;
float f1 = 0;



float Pitch_shift(float sample)
{
    //sample = IIR_filter_300Hz(sample);
    f1 = 1-f0;
    index_pitch_shift_write +=1;
index_pitch_shift_read+=2;
pitch_shift_array[index_pitch_shift_write] = sample;

if (index_pitch_shift_write >=1024)
{
    index_pitch_shift_write =0;
}
if (index_pitch_shift_read >=1024)
{
    index_pitch_shift_read =0;
}

s0 = f0 * pitch_shift_array[index_pitch_shift_read];
   s1 = f1 * pitch_shift_array[(index_pitch_shift_read + BUFFSIZE/2) % BUFFSIZE];
   return s0 + s1;





}
#define BufSize 1000
#define Overlap 100


int WtrP =0;
float Rd_P =0;
float Shift = 1.0;
float CrossFade =1;
float Buf[BufSize] = {0};
float Do_PitchShift(float sample) {
	

	//write to ringbuffer
	Buf[WtrP] = sample;

	//read fractional readpointer and generate 0° and 180° read-pointer in integer
	int RdPtr_Int = roundf(Rd_P);
	int RdPtr_Int2 = 0;
	if (RdPtr_Int >= BufSize/2) RdPtr_Int2 = RdPtr_Int - (BufSize/2);
	else RdPtr_Int2 = RdPtr_Int + (BufSize/2);

	//read the two samples...
	float Rd0 = (float) Buf[RdPtr_Int];
	float Rd1 = (float) Buf[RdPtr_Int2];

	//Check if first readpointer starts overlap with write pointer?
	// if yes -> do cross-fade to second read-pointer
	if (Overlap >= (WtrP-RdPtr_Int) && (WtrP-RdPtr_Int) >= 0 && Shift!=1.0f) {
		int rel = WtrP-RdPtr_Int;
		CrossFade = ((float)rel)/(float)Overlap;
	}
	else if (WtrP-RdPtr_Int == 0) CrossFade = 0.0f;

	//Check if second readpointer starts overlap with write pointer?
	// if yes -> do cross-fade to first read-pointer
	if (Overlap >= (WtrP-RdPtr_Int2) && (WtrP-RdPtr_Int2) >= 0 && Shift!=1.0f) {
			int rel = WtrP-RdPtr_Int2;
			CrossFade = 1.0f - ((float)rel)/(float)Overlap;
		}
	else if (WtrP-RdPtr_Int2 == 0) CrossFade = 1.0f;


	//do cross-fading and sum up
	sample = (Rd0*CrossFade + Rd1*(1.0f-CrossFade));

	//increment fractional read-pointer and write-pointer
	Rd_P += Shift;
	WtrP++;
	if (WtrP == BufSize) WtrP = 0;
	if (roundf(Rd_P) >= BufSize) Rd_P = 0.0f;

	return sample;


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
