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
#define white_green 5

int color[5][3]={{0,0,0},{0,255,255},{255,0,255},{255,255,0},{0,0,255},{255,0,30}};

 int counter = 0; 
 int aState;
 int aLastState;  
int current_effect=0;
bool setting=false;
bool last_pin4_state= false;
int setting_value=0;



int rotery_encoder()
{
     aState = gpio_get(12); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (gpio_get(11) != aState) { 
       counter ++;
     } else {
       counter --;
     }
    
   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state
   return counter;

}



bool repeating_timer_callback(struct repeating_timer *t)
{float sample = ADC_sample(); // get sample and put it in a variable
    sample = sample/4096.0;
    sample -=0.5;

//get button input
    bool current_pin4_state = gpio_get(4);
    
        if (current_pin4_state && !last_pin4_state) {
             setting = !setting;  // toggle the setting flag
        }
    if (setting){
    current_effect = rotery_encoder();
    }
    if (!setting)
    {
setting_value = rotery_encoder();
    }
   

    if (current_effect <0)
    {
        current_effect = 0;
        counter =0;
    }
    switch (current_effect)
    {
    case 1 :
        RGB_color(15,10,13,color,GREEN);
        sample = Delay(sample,1);
        break;

    case 2 :
    RGB_color(15,10,13,color,GREEN);
    sample = Delay(sample,1);
        break;

    case 3 :
    RGB_color(15,10,13,color,BLUE);
    sample =Tremelo(sample,1);
        break;

    case 4 :
    RGB_color(15,10,13,color,BLUE);
    sample =Tremelo(sample,1);
        break;

    case 5 :
sample = Flanger(sample,1);
        break;
    case 6:
    sample = Flanger(sample,1);
        break;
    case 7:
sample = overdrive(sample,0.11);
        break;
    case 8:
sample = overdrive(sample,0.11);
        break;
    case 9:
sample = Vibrato(sample,3);
        break;
    case 10:
sample = Vibrato(sample,3);
        break;
    default:
        break;
    }















 last_pin4_state = current_pin4_state;
    sample+=0.5;
    sample = sample *4096.0;
    
    output(sample,0);
    
    return true;

}



void main(){


   
    stdio_init_all();
    ADC_init(0,96);
    RGB_led_init(13,10,15);
    effect_init(0);

 


    struct repeating_timer timer;
    add_repeating_timer_us(12, repeating_timer_callback,NULL,&timer);
 
 aLastState = gpio_get(12);   


 
while (1) {

    
    tight_loop_contents();   
    
  

   
 

  
 }
  
}


    
    
    
    
    
    

    