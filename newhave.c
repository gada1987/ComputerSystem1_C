#include "system_sam3x.h"
#include "at91sam3x8.h"

extern int tick;

void Delay(int Value){
    int i;
    for(i=0; i<Value;i++)
        asm("nop");
}


unsigned char Read_Status_Display(void){
    unsigned char Temp;
    
    *AT91C_PIOC_ODR= (0xff<<2);
    *AT91C_PIOC_SODR= (1<<13);
    *AT91C_PIOC_CODR= (1<<12);
    *AT91C_PIOC_SODR= (1<<17);
    *AT91C_PIOC_CODR= (3<<15);
    Delay(10);
    Temp = (*AT91C_PIOC_PDSR & (0xff<<2))>> 2;
    *AT91C_PIOC_SODR= (1<<16);
    *AT91C_PIOC_SODR= (1<<15);
    *AT91C_PIOC_SODR= (1<<12);
    *AT91C_PIOC_CODR= (1<<13);
    return Temp;
}

void Write_Command_2_Display(unsigned char Command){
    while((Read_Status_Display()& (3<<0)) != (3<<0)){
        
    }
    *AT91C_PIOC_CODR= (0xff<<2);
    *AT91C_PIOC_SODR= (Command<<2);
    *AT91C_PIOC_CODR= (1<<13);
    *AT91C_PIOC_CODR= (1<<12);
    *AT91C_PIOC_OER= (0xff<<2);
    *AT91C_PIOC_SODR= (1<<17);
    *AT91C_PIOC_CODR= (1<<16);
    *AT91C_PIOC_CODR= (1<<14);
    Delay(10);
    *AT91C_PIOC_SODR= (1<<16);
    *AT91C_PIOC_SODR= (1<<14);
    *AT91C_PIOC_SODR= (1<<12);
    *AT91C_PIOC_ODR= (0xff<<2);
    
}

void Write_Data_2_Display(unsigned char Data){
    while((Read_Status_Display()& (3<<0))!= (3<<0)){
        
    }
    *AT91C_PIOC_CODR= (0xff<<2);
    *AT91C_PIOC_SODR= (Data<<2);
    *AT91C_PIOC_CODR= (1<<13);
    *AT91C_PIOC_CODR= (1<<12);
    *AT91C_PIOC_OER= (0xff<<2);
    *AT91C_PIOC_CODR= (1<<17);
    *AT91C_PIOC_CODR= (1<<16);
    *AT91C_PIOC_CODR= (1<<14);
    Delay(10);
    *AT91C_PIOC_SODR= (1<<16);
    *AT91C_PIOC_SODR= (1<<14);
    *AT91C_PIOC_SODR= (1<<12);
    *AT91C_PIOC_ODR= (0xff<<2);
    
}
void Init_Display(void){
    *AT91C_PIOD_CODR =(1<<0);
    tick = 0;
    while(tick<10);
    *AT91C_PIOD_SODR = (1<<0);
    Write_Data_2_Display(0x00);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x40); //Set text home address
    Write_Data_2_Display(0x00);
    Write_Data_2_Display(0x40);
    Write_Command_2_Display(0x42);  //Set graphic home address
    Write_Data_2_Display(40);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x41); // Set text area
    Write_Data_2_Display(0x1e);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x43); // Set graphic area
    Write_Command_2_Display(0x80); // text mode
    Write_Command_2_Display(0x94); // Text on graphic off
    
}