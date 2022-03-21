#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "key_pad.h"
#include "ljus.h"


int tick = 0;
int tick2= 0;
int tick3 = 0;
int tick4 = 0;
int tick5 = 0;
int Tc0_flag = 0;
int run_program= 0; // this is global variabel for extra material
void TC0_Handler(void){
    *AT91C_TC0_IDR =(1<<6);
    Tc0_flag = 1;
}
void PIOD_Handler(void){// for extra material
 *AT91C_PIOD_ISR;
  run_program = 1;
}

void SysTick_Handler(void){
    tick++;
    tick2++;
    tick3++;
    tick4++;
}
void delay_ms(int ms){
    tick3 = 0;
    while(tick3 < ms);
}
