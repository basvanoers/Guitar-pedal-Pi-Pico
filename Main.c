#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"

#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"


#include"lib/ADC/ADC.h"
#include "lib/PWM/PWM.h"
#include "lib/I2S/I2S.h"
#include"lib/Effects/Effects.h"

#include "PWM.pio.h"
#include "I2S.pio.h"
#include "pico/audio_i2s.h"


#define SINE_WAVE_TABLE_LEN 2048
#define SAMPLES_PER_BUFFER 1

static int16_t sine_wave_table[SINE_WAVE_TABLE_LEN];


struct audio_buffer_pool *init_audio() {

    static audio_format_t audio_format = {
            .format = AUDIO_BUFFER_FORMAT_PCM_S16,


            .sample_freq = 48000,

            .channel_count = 1,
    };

    static struct audio_buffer_format producer_format = {
            .format = &audio_format,
            .sample_stride = 2
    };

    struct audio_buffer_pool *producer_pool = audio_new_producer_pool(&producer_format, 3,
                                                                      SAMPLES_PER_BUFFER); // todo correct size
    bool __unused ok;
    const struct audio_format *output_format;

    struct audio_i2s_config config = {
            .data_pin = PICO_AUDIO_I2S_DATA_PIN,
            .clock_pin_base = PICO_AUDIO_I2S_CLOCK_PIN_BASE,
            .dma_channel = 0,
            .pio_sm = 0,
    };

    output_format = audio_i2s_setup(&audio_format, &config);
    if (!output_format) {
        panic("PicoAudio: Unable to open audio device.\n");
    }

    ok = audio_i2s_connect(producer_pool);
    assert(ok);
    audio_i2s_set_enabled(true);


    return producer_pool;
}


#define Fs 48000
#define Ts (1/Fs)



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
#define OFF 10
#define DARK_GREEN 11

typedef struct {
float Delay ;
float Gain;
float Tremelo;
float clipping;
float vibrato;
float shift;
float tanh_Gain;
float Bass;
float Mid;
float Treble;

} Settings ;
Settings waardes = {
    .Gain = 1,
    .Delay = 1,
    .Tremelo = 1,
    .clipping = 0.11,
    .vibrato = 1,
    .shift = 2,
    .tanh_Gain =1,
    .Mid =10,
    .Bass=5,
    .Treble=8
};

int color[12][3]={{0,0,0},{0,255,255},{255,0,255},{255,255,0},{0,0,255},{255,0,30},{0,255,0},{191,228,189},{36,78,147},{0,107,255},{255,255,255},{239,204,226}};

 int counter = 0; 
 int aState;
 int aLastState;  
int current_effect=0;
bool setting=true;
bool last_pin4_state= false;
int setting_value=0;
int remeber_effect=0;
int j=0;
uint DMA_channel;




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
{
    //dma_channel_wait_for_finish_blocking(DMA_channel);
    float sample = ADC_sample(); // get sample and put it in a variable
    sample = sample/4096.0;
    sample -=0.5;
    sample = Gain(sample,6);
    //sample = Treble(sample,1)+Mid(sample,1);
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
   






    
    switch (current_effect)
    {
    //Gain
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
    //Delay
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
    //Delay
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
    //Tremelo
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
    //Tremelo
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
    //Flanger
    case 5 :
    RGB_color(15,10,13,color,ORANGE);
    sample = Flanger(sample,1);
        break;
    //Flanger
    case 6:
    RGB_color(15,10,13,color,ORANGE);
    sample = Flanger(sample,1);
        break;
    //Overdrive
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
    //Overdrive
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
    //Vibrato
    case 9:
    RGB_color(15,10,13,color,BEIGE);
//sample = Vibrato(sample,waardes.vibrato);
        break;
    //Vibrato
    case 10:
    RGB_color(15,10,13,color,BEIGE);
//sample = Vibrato(sample,waardes.vibrato);
        break;
    //Pitch_shift
    case 11:
    if(setting)
    {
        RGB_color(15,10,13,color,DARK_PURPLE);
    }
    else{
        waardes.shift = setting_value*0.5;
    }
    sample = Do_PitchShift(sample);
    break;
    //Pitch_shift
    case 12:
    if(setting)
    {
        RGB_color(15,10,13,color,DARK_PURPLE);
    }
    else{
        waardes.shift = setting_value*0.5;
    }
    sample = Do_PitchShift(sample);
    break;
    //Distortion
    case 13:
    if(setting)
    {
        RGB_color(15,10,13,color,DARK_GREEN);
    }
    else{
        waardes.tanh_Gain = setting_value;
    }
    sample = tanh(sample*waardes.tanh_Gain);
    sample = Gain(sample,0.8);
    break;
    //Distortion
    case 14:
    if(setting)
    {
        RGB_color(15,10,13,color,DARK_GREEN);
    }
    else{
        waardes.tanh_Gain = setting_value;
    }
    sample = tanh(sample*waardes.tanh_Gain);
    sample = Gain(sample,0.8);
    break;

    case 15:
    sample = Chorus(sample,1);
    RGB_color(15,10,13,color, OFF);

    break;
    case 16:
    sample = Chorus(sample,1);
    RGB_color(15,10,13,color, OFF);

    break;
    case 17:
    if(setting)
    {
        RGB_color(15,10,13,color,DARK_GREEN);
    }
    else{
        waardes.Bass = setting_value;
    }
   
    
    break;
    case 18:
    if(setting)
    {
        RGB_color(15,10,13,color,DARK_GREEN);
    }
    else{
        waardes.Bass = setting_value;
    }
    
    
    break;
    case 19:
    if(setting)
    {
        RGB_color(15,10,13,color,BEIGE);
    }
    else{
        waardes.Treble = setting_value;
    }
  
    
    break;
     case 20:
    if(setting)
    {
        RGB_color(15,10,13,color,BEIGE);
    }
    else{
        waardes.Treble = setting_value;
    }
   
    
    break;
     case 21:
    if(setting)
    {
        RGB_color(15,10,13,color,RED);
    }
    else{
        waardes.Mid = setting_value;
    }
   
    break;
    case 22:
    if(setting)
    {
        RGB_color(15,10,13,color,RED);
    }
    else{
        waardes.Mid = setting_value;
    }
    
    
    break;
    default:
        break;
    }
    last_pin4_state = current_pin4_state;
 
    sample+=0.5;
    sample = sample *4096.0;
    if (sample < 1000)
    {
        sample = 0;
    }
    output(sample,0);
    
    return true;

}



void main(){


   
    //stdio_init_all();
    
    RGB_led_init(13,10,15);
    effect_init(0);
    ADC_init(0,63); 
 


   // struct repeating_timer timer;
    //add_repeating_timer_us(12, repeating_timer_callback,NULL,&timer);
 
 aLastState = gpio_get(12);  
// DMA_channel = 
 for (int i = 0; i < SINE_WAVE_TABLE_LEN; i++) {
        sine_wave_table[i] = 32767 * cosf(i * 2 * (float) (M_PI / SINE_WAVE_TABLE_LEN));
    }

    struct audio_buffer_pool *ap = init_audio();
    uint32_t step = 0x200000;
    uint32_t pos = 0;
    uint32_t pos_max = 0x10000 * SINE_WAVE_TABLE_LEN;
    uint vol = 128;
    int i =0;
    bool up= true;
    while (true) {
       
        struct audio_buffer *buffer = take_audio_buffer(ap, true);
        int16_t *samples = (int16_t *) buffer->buffer->bytes;
        for (uint i = 0; i < buffer->max_sample_count; i++) {
            samples[i] = (vol * sine_wave_table[pos >> 16u]) >> 8u;
            pos += step;
            if (pos >= pos_max) pos -= pos_max;
        }

       
        buffer->sample_count = buffer->max_sample_count;
        give_audio_buffer(ap, buffer);
    }

 
  
}


    
    
    
    
    
    

    