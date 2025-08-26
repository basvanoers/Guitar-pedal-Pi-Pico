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
    gpio_set_dir(4, 0);
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

float Flanger(float sample, float freq)
{
    delay_line_add(sample);
float Output = sample + delay_line_get(LFO(freq,480))*0.8;

    return Output;
}
float Chorus(float sample,float freq)
{
    delay_line_add(sample);
    float Output = sample+delay_line_get(LFO(freq,480))*0.8+delay_line_get(LFO(freq+2,480))*0.7+delay_line_get(LFO(freq+4,480))*0.6+delay_line_get(LFO(freq+6,480))*0.5;
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

#define BufSize 1000
#define Overlap 100

float Buf[BufSize];

int WtrP =0;;
float Rd_P =0.0f;
float Shift =1.95;
float CrossFade = 1.0f;
float a0, a1, a2, b1, b2, hp_in_z1, hp_in_z2, hp_out_z1, hp_out_z2;

//0°
	

    float Do_HighPass (float inSample) {
	//300Hz high-pass, 96k
	a0 =0.9850241702992f;
	a1 = -1.970048340598f;
	a2 = 0.9850241702992f;
	b1 = -1.969824052547f;
	b2 = 0.9702726286499f;

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

float Do_PitchShift(float sum) {
	
	//sum up and do high-pass
	sum=Do_HighPass(sum);

	//write to ringbuffer
	Buf[WtrP] = sum;

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
	sum = (Rd0*CrossFade + Rd1*(1.0f-CrossFade));

	//increment fractional read-pointer and write-pointer
	Rd_P += Shift;
	WtrP++;
	if (WtrP == BufSize) WtrP = 0;
	if (roundf(Rd_P) >= BufSize) Rd_P = 0.0f;

	return sum;


}
float Bass_a1,Bass_a2,Bass_a0,Bass_b0,Bass_b1,Bass_b2;
float x[3]={0};
float y[3] = {0};
float Bass(float sample, float Gain)
{
    x[0] = sample;

Bass_a0 = 0.0003460440697434;
Bass_a1 =0.0006920881394868;
Bass_a2=0.0003460440697434;
Bass_b0 =1;
Bass_b1=   -1.946697327641;
Bass_b2=   0.9480815039196;
y[0] = Bass_a0*x[0]+Bass_a1*x[1]+Bass_a2*x[2]-Bass_b1*y[1] - Bass_b2*y[2];
x[1] = x[0];
x[2] = x[1];
y[1] = y[0];
y[2] = y[1];

return y[0]*Gain;
}

float Treble_a1,Treble_a2,Treble_a0,Treble_b0,Treble_b1,Treble_b2;
float Treble(float sample,float Gain)
{
 x[0] = sample;

Treble_a0 =0.07023598201045;
Treble_a1 =0.0006920881394868;
Treble_a2=-0.07023598201045;
Treble_b0 =1;
Treble_b1=   -1.81732290134;
Treble_b2=   0.8595280359791;
y[0] = Treble_a0*x[0]+Treble_a1*x[1]+Treble_a2*x[2]-Treble_b1*y[1] - Treble_b2*y[2];
x[1] = x[0];
x[2] = x[1];
y[1] = y[0];
y[2] = y[1];

return y[0]*Gain;
}

float Mid_a1,Mid_a2,Mid_a0,Mid_b0,Mid_b1,Mid_b2;
float Mid(float sample,float Gain)
{
 x[0] = sample;

Mid_a0 = 0.05357633220485;
Mid_a1 =0;
Mid_a2=-0.05357633220485;
Mid_b0 =1;
Mid_b1=   -1.887463785371;
Mid_b2=   0.8928473355903;
y[0] = Mid_a0*x[0]+Mid_a1*x[1]+Mid_a2*x[2]-Mid_b1*y[1] - Mid_b2*y[2];
x[1] = x[0];
x[2] = x[1];
y[1] = y[0];
y[2] = y[1];

return y[0]*Gain;
}

