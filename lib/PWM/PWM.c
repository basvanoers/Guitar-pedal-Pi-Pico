#include "pico/stdlib.h"
#include "hardware/pwm.h"
#define PWM_WRAP 4095
void pwm_init_pin(uint gpio_pin) {
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
    uint slice=pwm_gpio_to_slice_num (gpio_pin); 
    uint channel=pwm_gpio_to_channel (gpio_pin);  
    pwm_set_enabled (slice, 1);
    
    pwm_set_wrap (slice, 4097);
          
}
void RGB_led_init(uint R,uint B ,uint G)
{
    gpio_set_function(R, GPIO_FUNC_PWM);
    gpio_set_function(B, GPIO_FUNC_PWM);
    gpio_set_function(G, GPIO_FUNC_PWM);
    uint slice_R=pwm_gpio_to_slice_num (R); 
    uint channel_R=pwm_gpio_to_channel (R);  
    uint slice_B=pwm_gpio_to_slice_num (B); 
    uint channel_B=pwm_gpio_to_channel (B); 
    uint slice_G=pwm_gpio_to_slice_num (G); 
    uint channel_G=pwm_gpio_to_channel (G); 

   

     pwm_set_enabled (slice_R, 1);
     pwm_set_enabled (slice_B, 1);
      pwm_set_enabled (slice_G, 1);

    pwm_set_wrap (slice_R, 255);
     pwm_set_wrap (slice_B, 255);
      pwm_set_wrap (slice_G, 255);

}
void RGB_color(uint R_pin,uint B_pin,uint G_pin,int color[][3],int Color)
{
    pwm_set_gpio_level (R_pin,color[Color][0] );
    pwm_set_gpio_level(B_pin,color[Color][1]);
    pwm_set_gpio_level(G_pin,color[Color][2]); 
}

// Set duty cycle using a 12-bit value (0–4095)
void output(uint16_t value,uint gpio_pin) {
    pwm_set_gpio_level (gpio_pin,value ); 
}