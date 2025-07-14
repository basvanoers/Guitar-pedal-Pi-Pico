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
#define PURPLE 6
#define DARK_PURPLE 7
#define BEIGE 8
#define ORANGE 9
typedef struct {
float Delay ;
float Gain;
float Tremelo;
float clipping;
float vibrato;

} Settings ;
Settings waardes = {
    .Gain = 1,
    .Delay = 1,
    .Tremelo = 1,
    .clipping = 0.11,
    .vibrato = 1
};

int color[10][3]={{0,0,0},{0,255,255},{255,0,255},{255,255,0},{0,0,255},{255,0,30},{0,255,0},{191,228,189},{36,78,147},{0,107,255}};

 int counter = 0; 
 int aState;
 int aLastState;  
int current_effect=0;
bool setting=true;
bool last_pin4_state= false;
int setting_value=0;
int remeber_effect=0;
int j=0;





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
    if(j ==0)
    {
        counter = remeber_effect;
        j++;
    }
    current_effect = rotery_encoder();
    remeber_effect = current_effect;
    }

    if (!setting)
    {
       
        setting_value = rotery_encoder();
        RGB_color(15,10,13,color,RED);
        j=0;
    }

    if (setting_value<0)
    {
        setting_value=0;
    }

    if (current_effect <0)
    {
        current_effect = 0;
        counter =0;
    }
   






    if (true){
    switch (current_effect)
    {
    
    case 0:
    if (setting)
    {
 RGB_color(15,10,13,color,WHITE);
    }
    else
    {
        waardes.Gain = setting_value*0.1;
    }
    sample = Gain(sample,waardes.Gain);
   
        break;
    case 1 :
    if (setting)
    {
        RGB_color(15,10,13,color,GREEN);
    }
    else{
        waardes.Delay = setting_value*0.1;
        if(waardes.Delay<1)
        {
            waardes.Delay =1;
        }
    }
        
        sample = Delay(sample,waardes.Delay);
        break;

    case 2 :
    if (setting)
    {
        RGB_color(15,10,13,color,GREEN);
    }
    else{
        waardes.Delay = setting_value*0.1;
        if (waardes.Delay <1)
        {
            waardes.Delay=1;
        }
    
       
    }
     sample = Delay(sample,waardes.Delay);
        
        break;

    case 3 :
    if(setting)
    {
RGB_color(15,10,13,color,BLUE);
    }
    else{
        waardes.Tremelo = setting_value;
    }
    
    sample =Tremelo(sample,waardes.Tremelo);
        break;

    case 4 :
     if(setting)
    {
RGB_color(15,10,13,color,BLUE);
    }
    else{
        waardes.Tremelo = setting_value;
    }
    
    sample =Tremelo(sample,waardes.Tremelo);
        break;

    case 5 :
    RGB_color(15,10,13,color,ORANGE);
sample = Flanger(sample,1);
        break;
    case 6:
    RGB_color(15,10,13,color,ORANGE);
    sample = Flanger(sample,1);
        break;
    case 7:
 if(setting)
    {
RGB_color(15,10,13,color,PURPLE);
    }
    else{
        waardes.clipping = setting_value*0.01;
    }
    
sample = overdrive(sample,waardes.clipping);
        break;
    case 8:
   if(setting)
    {
RGB_color(15,10,13,color,PURPLE);
    }
    else{
        waardes.clipping = setting_value*0.01;
    }
    
sample = overdrive(sample,waardes.clipping);
        break;
    case 9:
    RGB_color(15,10,13,color,BEIGE);
sample = Vibrato(sample,waardes.vibrato);
        break;
    case 10:
    RGB_color(15,10,13,color,BEIGE);
sample = Vibrato(sample,waardes.vibrato);
        break;
    case 11:
    RGB_color(15,10,13,color,DARK_PURPLE);
    sample = Pitch_shift(sample);
    break;
    case 12:
    RGB_color(15,10,13,color,DARK_PURPLE);
    sample = Pitch_shift(sample);
    break;
    default:
        break;
    }
}















 last_pin4_state = current_pin4_state;
 
    sample+=0.5;
    sample = sample *4096.0;
    if (sample < 100)
    {
        sample = 0;
    }
    output(sample,0);
    
    return true;

}



void main(){


   
    //stdio_init_all();
    ADC_init(0,50000);
    RGB_led_init(13,10,15);
    effect_init(0);

 


    struct repeating_timer timer;
    add_repeating_timer_us(12, repeating_timer_callback,NULL,&timer);
 
 aLastState = gpio_get(12);   


 
while (1) {

    
    tight_loop_contents();   
    
  

   
 

  
 }
  
}


    
    
    
    
    
    

    