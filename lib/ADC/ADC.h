

// Channel 0 is GPIO26
//#define CAPTURE_CHANNEL 0
#define LED_PIN 25
#define NSAMP 1000
#include <stdio.h>
#include <stdint.h>



int ADC_init(int CAPTURE_CHANNEL, int CLOCK_DIV);
float ADC_sample(void);