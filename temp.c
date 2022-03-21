#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "Trap.h"
#include "print.h"
#include <stdio.h>
#include "newhave.h"
#include "key_pad.h"


float Temparature;
float Temp_buffer[10080];
int Temp_buffer_index= 0;


#define TC0_RA    (unsigned int *)     0x40080014 // (TC0) Register A
#define TC0_RB    (unsigned int *)     0x40080018 // (TC0) Register B
//unsigned int RB = *AT91C_TC0_RB;
//unsigned int RA = *AT91C_TC0_RA;

void init_temp(void){
    *AT91C_PIOB_CODR = (1<<25);
    *AT91C_PIOB_OER = (1<<25);// F√∑R ATT STYRA VOLT
    
    tick = 0;
    while(tick<10);
    *AT91C_PIOB_SODR = (1<<25);
    *AT91C_PIOB_ODR = (1<<25);
}

void Delay_temp(int Value){
    int i;
    for(i=0; i<Value;i++)
        asm("nop");
}

void pulse(void){
    *AT91C_PIOB_CODR = (1<<25);
    *AT91C_PIOB_OER = (1<<25);// F√∑R ATT STYRA VOLT
    
    Delay_temp(25);
    *AT91C_PIOB_SODR = (1<<25);
    *AT91C_PIOB_ODR = (1<<25);
    *AT91C_TC0_CCR =(1<<2);
    *AT91C_TC0_SR;
    *AT91C_TC0_IER =(1<<6);
    
}

void logging_temp(void){
    Temp_buffer[Temp_buffer_index]= Temparature; //mäta och spara in arr på plats postion
    Temp_buffer_index++;
    if(Temp_buffer_index >10080){
        Temp_buffer_index = 0;
    }
}

void days_of_week(void){
 float min = 100.0;
 float max = 0.0;
 float sum = 0.0;
 float ave;
 int index;
  for(int p = 0; p < 3; p++){
     for(int k= 0; k< 3; k++){
       move_ptr(rows_postion[p]+ 1+k,1);
       print_str(Statistic_str[k]);
     }
   }
  
   for(int i = 0; i < 7; i++){  
     sum = 0.0;
     min= 100.0;
     max= 0.0;
     move_ptr(rows_postion[i], columns_positon[i]);
     print_str( Days_str[i]);
     for(int j = 0 ; j < 1440; j++){   //  Get_statistics for monday
       index = i * 1440 +j;
       sum += Temp_buffer[index];
         if(Temp_buffer[index]< min){
            min = Temp_buffer[index];
         }
         if(Temp_buffer[index]> max){
            max = Temp_buffer[index];
         }
      }
      ave = sum / 1440 ;
      move_ptr(rows_postion[i]+1, columns_positon[i]);
      print_float(min);
      move_ptr(rows_postion[i]+2, columns_positon[i]);
      print_float(max);
      move_ptr(rows_postion[i]+3, columns_positon[i]);
      print_float(ave);
   }
}