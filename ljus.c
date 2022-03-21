#include "system_sam3x.h"
#include "at91sam3x8.h"
#include <stdio.h>
#include "newhave.h"
#include "key_pad.h"
#include "print.h"
#include "Trap.h"
#include "temp.h"

int sun_pos;
float light_sensor(void){
    int ljus;
    *AT91C_ADCC_CHER= (1<<2); // (ADC) Channel 1
    *AT91C_ADCC_CR= (1<<1);   // ADC Start Conversion
    while((*AT91C_ADCC_SR & (1<<24)) == 0);
    ljus = (*AT91C_ADCC_LCDR & (0xfff)); // (ADC) Last Data Converted
    return( (3.3/4095)*ljus);
}


void servo(int deg){
    unsigned int ghada;
    float tid;
    tid = ((2.3 - 0.7)/(120))* deg + 0.7;
    ghada=(unsigned int)(2625*tid);
    *AT91C_PWMC_CH1_CDTYR = ghada;
}

    
void sun_position(void){
    float arr[120]; // 120 grad servo runt
    float stor = -1.0; // jag kan l‰gga 0 men det ‰r mer s‰krare
    for(int i = 0; i< 120; i++){
        servo(i);
        arr[i] = light_sensor();
        delay_ms(20); // 20ms * 120= 2.4 s
    }
    for(int j = 0 ; j<120; j++){
        if(arr[j]> stor){
            stor = arr[j];
            sun_pos = j;
        }
    }
    servo(sun_pos);
}