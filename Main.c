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


//color array for rotery encoder
#define WHITE 0
#define RED 1
#define BLUE 3
#define GREEN 2
#define YELLOW 4

int color[5][3]={{0,0,0},{0,255,255},{255,0,255},{255,255,0},{0,0,255}};


typedef struct {
int Delay;
float Gain;
float Tremelo;
float clipping;

} Settings ;







   





//function for getting the value for the rotary encoder
int counter = 0; 
int aState;
int aLastState; 
int effect=0; 
int rotery_encoder(absolute_time_t *last_change_time)
{
    aState = gpio_get(11);
    if (aState != aLastState) {
        absolute_time_t now = get_absolute_time();
        if (absolute_time_diff_us(*last_change_time, now) > 1000) { // debounce
            *last_change_time = now;
            aLastState = aState;
            if (gpio_get(12) != aState) {
                counter++;
            } else {
                counter--;
            }
            return counter;
        }
    }
    return -999; // No change
}







void main(){


    
    stdio_init_all();
    ADC_init(0,96);
    RGB_led_init(13,10,15);
    effect_init(0);
    
   // Reads the initial state of the outputA
   aLastState = gpio_get(11);
   
   int old_counter =0;
   int effect_1=0;
   bool last_pin4_state = false;
 


 int last_counter = 0;
 int last_counter1=0;
int change_count = 0;
absolute_time_t last_change_time = get_absolute_time();

int current_effect = 0;
int value=1;
bool setting = false;

int setting_value=1;
int a=0;


int64_t next_sample_time_us = time_us_64();





 
while (1) {
    
    
    float sample = ADC_sample(); // get sample and put it in a variable
    sample = sample/4096.0;
    sample -=0.5;
    
    sample+=0.5;
    sample = sample *4096.0;
    output(sample,0);
    
    
    
    
    
    

    
    
    //get rotery encoder input
    /*int new_counter = rotery_encoder(&last_change_time);
    if (new_counter != -999 && new_counter != last_counter&&setting==false) {
        int diff = new_counter - last_counter;
        current_effect += diff;  // increment or decrement
        last_counter = new_counter;
        
    }
   
    if (new_counter != -999&& new_counter!=last_counter1&&setting==true)
    {
        int diff1= new_counter-last_counter1;
        setting_value +=diff1;
        last_counter1= new_counter;
    }


    //get button input
    bool current_pin4_state = gpio_get(4);
    
        if (current_pin4_state && !last_pin4_state) {
             setting = !setting;  // toggle the setting flag
             setting_value=1;
    
            
   
}



    if (current_effect >=1)
    {
        current_effect=0;
        
    }

    //gain effect 
    if(current_effect==0||current_effect==-1)
    {
      
        if (setting==false){
        RGB_color(15,10,13,color,RED);
        }
        if (setting==true){
            RGB_color(15,10,13,color,YELLOW);
            Gain = -setting_value*0.1; 
            
        }
        sample = gain(sample,Gain);
    }

    //delay effect
    if(current_effect==-2||current_effect==-3)
    {
        if (setting==false){
        RGB_color(15,10,13,color,GREEN);
        }
        if (setting==true){
            RGB_color(15,10,13,color,YELLOW);
            Delay = -setting_value;
            if (Delay >= 4*40){
                Delay = 4*40;
            } 
            
        }
        sample = delay(sample,Delay*1200);
    }
    //Tremelo effect
    if(current_effect==-4||current_effect==-5)
    {   if (setting ==false)
        { 
        RGB_color(15,10,13,color,BLUE);
        }
        if (setting == true){
              RGB_color(15,10,13,color,YELLOW);
              Tremelo = -setting_value*0.1;
        }

        sample = tremelo(sample,Tremelo);
    }

    if(current_effect==-6||current_effect==-7)
    {   if (setting ==false)
        { 
        RGB_color(15,10,13,color,WHITE);
        }
        if (setting == true){
              RGB_color(15,10,13,color,YELLOW);
              clipping = setting_value*0.1;
              printf("%f\n",setting_value);
        }

        sample = soft_clipping_distortion(sample,clipping);
    }
     if(current_effect==-8||current_effect==-9)
    {   if (setting ==false)
        { 
        RGB_color(15,10,13,color,WHITE);
        }
        if (setting == true){
              RGB_color(15,10,13,color,YELLOW);
              clipping = setting_value*0.1;
              printf("%f\n",setting_value);
        }

        sample = hard_clipping_distortion(sample,clipping);
    }

   
    if(current_effect==-10)
    {
        RGB_color(15,10,13,color,WHITE);
    }
    if (current_effect <=-12)
    {
        current_effect = 0;
    }
    last_pin4_state = current_pin4_state;
    sample+=0.5;
    sample = sample *4096.0;
    output(sample,0);
   
    
*/
}
//RGB_color(15,10,13,color,GREEN);



}
