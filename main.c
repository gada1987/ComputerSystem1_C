
#include "system_sam3x.h"
#include "at91sam3x8.h"
#include <stdio.h>
#include "newhave.h"
#include "key_pad.h"
#include "print.h"
#include "Trap.h"
#include "temp.h"
#include "ljus.h"

void move_ptr(int x, int y);
void print_str(char *ch);
void print_float(float value);
int check =0;
int key ;
int new_button= -1;
int old_button;

enum {init,welcome,main_menu,statistic,find_sun,set_alarm,mode,status
}State_Machine;


void init_pins(){
    
    *AT91C_PMC_PCER=(1<<13);//STARTA Peripherals CLOCK PORT C
    *AT91C_PMC_PCER =(1<<14);//STARTA Peripherals CLOCK PORT D
    *AT91C_PMC_PCER =(1<<12);// STARTA PERIPHERSLS CLOCK PORT B
    *AT91C_PMC_PCER=(1<<11);//STARTA Peripherals CLOCK PORT A light sensor
    *AT91C_PMC_PCER =(1<<27);// instance id for Timer 0
    
    *AT91C_PIOD_PER = (1<<3);// port D pin 3 kopplad till led lampa
    *AT91C_PIOD_PER = (1<<2);//PORT D pin2  kopplad till keypad 74chip OE
    *AT91C_PIOD_OER = (1<<3);// port D pin 3 kopplad till led lampa
    *AT91C_PIOD_OER = (1<<2);//PORT D pin2  kopplad till keypad 74chip OE
    
    *AT91C_PIOC_PER = (0xff<<12);// PORT C CONTROLL BUS FOR DISPLAY
    *AT91C_PIOC_OER= (0xff<<12);// out put enable
    *AT91C_PIOC_SODR= (0xff<<12);
    
    *AT91C_PIOC_PER = (0xff<<2); //port C pin 2 till 9 kopplad till databus
    
    *AT91C_PIOD_OER = (1<<0);
    *AT91C_PIOB_PER = (1<<25);//port B pin 25 kopplad till TEMP SENSOR
    *AT91C_TC0_CCR = (1<<0);
    *AT91C_TC0_CMR = (3<<17);
    NVIC_ClearPendingIRQ( TC0_IRQn);
    NVIC_SetPriority( TC0_IRQn, 0);
    NVIC_EnableIRQ(TC0_IRQn);
    //light sensor
    *AT91C_PIOA_PER = (1<<4);//port A pin 4 kopplad till light SENSOR
    *AT91C_PIOA_PER = (1<<3);//port A pin 3 kopplad till light SENSOR
    *AT91C_PMC_PCER1 = (1<<5);// Peripheral Clock Enable Register 32:63 PERI_ID
    *AT91C_ADCC_MR = (1<<9); // (ADC) Selected TRGSEL = TIAO1, parsescaler to 2
    //servo
    *AT91C_PIOB_PDR = (1<<17);//lÂt peripheral controllera port b pin 17
    *AT91C_PMC_PCER1 = (1<<4);// periphial clock Enable PWM
    *AT91C_PIOB_ABMR = (*AT91C_PIOB_ABMR |(1<<17));
    *AT91C_PWMC_ENA = (1<<1);// enable channel 1
    *AT91C_PWMC_CH1_CMR= 5;
    *AT91C_PWMC_CH1_CPRDR=(52500);
    *AT91C_PWMC_CH1_CDTYR =(2625);
    //bush_button extra
    *AT91C_PIOD_PER = (1<<9);// port D pin 9 kopplad till bush_buton
}

void Run_Task(void){
   
    SystemInit();
    SysTick_Config(84000); 
    init_pins();
    key_pad();
    Init_Display();
    init_temp();

    light_sensor();

    Temparature =  25.1;

  
   State_Machine= init;
    while(1){
       if(tick2 > 1000){
        old_button = new_button;
        new_button = key_pad();
        
      switch(State_Machine){
      case init:
         init_pins();
         State_Machine= welcome;
         clean();
      break;
      case welcome:

       Welcome_Menu();
        if(new_button== 12){
        State_Machine= main_menu;
        clean();
        }
      break;
      case main_menu:
        Print_Menu();
         if(old_button== -1){
            if(new_button == 1){
            State_Machine= statistic;
             clean();
            }
            else if(new_button == 2){
              State_Machine= find_sun;
               clean();
         }
         else if(new_button == 3){
          State_Machine= set_alarm;
          for(int j= 0; j<6; j++){
            alarm_buffer[j]= 0;
          }
           clean();
    }
       else if(new_button == 4){
        State_Machine= mode;
         clean();
    }
     else if(new_button == 5){
        State_Machine= status;
         clean();
    }
         
} 
    break;
    
     case statistic:
       Statistic_menu();
       if(old_button == -1){
       if(new_button == 10){
       State_Machine= main_menu;
       clean();
        }
    }
     break;
              
    case find_sun:
    sun_position_menu();
    if(old_button == -1){
    if(new_button == 10){
    State_Machine= main_menu;
     clean();
     servo_done = 0;
     }
    }
    break;
 
    case set_alarm:
    Alarm_menu();
    if(old_button == -1){
      if(new_button == 10){
         State_Machine= main_menu;
         clean();
      }
      else if(new_button == 12){
        upper_limit =alarm_buffer[0]*10.0+ alarm_buffer[1]+(float)(alarm_buffer[2])/10.0;
        lower_limit =alarm_buffer[3]*10.0+ alarm_buffer[4]+(float)(alarm_buffer[5])/10.0;
        State_Machine= main_menu;
        clean();
      }
   }
   
    break;
    case mode:
    Mode_menu();
    if(old_button == -1){if(new_button == 10){
      State_Machine= main_menu;
      clean();
    }
    else if(new_button == 12){
      sample_time = selected_mode;
       
        State_Machine= main_menu;
        clean();
      }
  
}
    break;
    case status:
    Status_menu();
    if(old_button == -1){
    if(new_button == 10){
    State_Machine= main_menu;
    clean();
  }
}
    break;
      }
      if(tick > 500){
        tick = 0;
        pulse();
      }
      if(tick4 > sample_time){ //logging_temp every fast_mode
            tick4 = 0;
            logging_temp();

        }


        if(Tc0_flag == 1){
          Tc0_flag = 0;
          Temparature =  (float)(*TC0_RB - *TC0_RA)/210.0 - 273.15;
        }

    }

}
}
//extra material
void main(void){
  int tick5=0;
   NVIC_ClearPendingIRQ( PIOD_IRQn);//use the function to clear pending interupts
   NVIC_SetPriority( PIOD_IRQn, 0); // to set priorty
   NVIC_EnableIRQ(PIOD_IRQn);//to set en able interrupt
   *AT91C_PIOD_IER= (1<<9); // to nable interrupt
   if(run_program == 0){
   *AT91C_PIOD_CODR= (1<<0);// clear reset display o sleep
   }
   else if(run_program ==1){
     if(tick5<=30000){
    *AT91C_PIOD_SODR= (1<<0);
     Run_Task();// this function run our program
     }
   }
   if(tick5 >30000){
     run_program = 0;
   }
  
}